#include <boost/bind.hpp>

#include "Cake/Threading/ScopedLock.hpp"
#include "ActionPerformer.hpp"

using namespace Server::Game::Actions;

ActionPerformer::ActionPerformer(
        IActionFactory & actionFactory,
        Common::Game::Universe & universe, 
        Server::Game::IPlayerContainer & playerContainer) :
    m_actionFactory(actionFactory),
    m_universe(universe),
    m_playerContainer(playerContainer)
{
}

void ActionPerformer::perform(Server::Network::IConnection & connection, 
    Common::Game::IPlayer & player, unsigned id, unsigned parameter)
{
    aquireGlobalCooldown(player.getId());
    auto action = m_actionFactory.create(connection, player, id, parameter);
    auto timeToFinish = action->start();

    if (timeToFinish == Common::Game::TimeValue(0, 0))
    {
        LOG_DEBUG << "Action doesn't have execution time and will be finished immidiately";
        action->finish();
    }
    else
    {
        LOG_DEBUG << "Action started, will be finished after " << timeToFinish << "secs";

        auto & selectedObject = player.getFocusedObject();

        Common::Messages::ActionStarted actionStarted;
        actionStarted.actionId = id;
        actionStarted.objectId = selectedObject.getId();
        connection.send(actionStarted);

        unsigned internalId = 0;
        {
            Cake::Threading::ScopedLock lock(m_ongoingActionsMutex);
            internalId = m_idGenerator.generate();
            m_ongoingActions.insert(std::make_pair(internalId, action));
        }
        m_time->createTimer(timeToFinish, boost::bind(
            &ActionPerformer::actionTimerExpired, this, 
            internalId, player.getId(), selectedObject.getId(), id));
    }
}

void ActionPerformer::aquireGlobalCooldown(unsigned playerId)
{
    auto ret = m_playerGlobalCooldowns.insert(playerId);
    if (ret.second)
    {
        LOG_DEBUG << "Global cooldown activated on player: " << playerId;
        m_time->createTimer(Common::Game::TimeValue(1, 0), boost::bind(&ActionPerformer::globalCooldownExpired, this, playerId));
    }
    else
    {
        LOG_WARN << "Global cooldown is already active on player: " << playerId << ", this might suggest hack attempt";
        throw std::runtime_error("global cooldown is active");
    }
}

void ActionPerformer::globalCooldownExpired(unsigned playerId)
{
    LOG_DEBUG << "Global cooldown expired for player: " << playerId;

    auto & connection = m_playerContainer.getConnectionById(playerId);
    Common::Messages::GlobalCooldownExpired globalCooldownExpired;
    connection.send(globalCooldownExpired);

    size_t elementsErased = m_playerGlobalCooldowns.erase(playerId);
    if (elementsErased == 0)
    {
        throw std::runtime_error("expired nonexisting global cooldown");
    }
}

void ActionPerformer::actionTimerExpired(unsigned internalId, unsigned playerId, unsigned objectId, unsigned actionId)
{
    Cake::Threading::ScopedLock lock(m_ongoingActionsMutex);

    auto it = m_ongoingActions.find(internalId);
    if (it == m_ongoingActions.end())
    {
        throw std::runtime_error("action timer expired but there is no such active action");
    }

    Common::Messages::ActionFinished actionFinished;
    actionFinished.objectId = objectId;
    actionFinished.actionId = actionId;

    auto & connection = m_playerContainer.getConnectionById(playerId);
    connection.send(actionFinished);

    boost::shared_ptr<IAction> action = it->second;
    action->finish();

    m_ongoingActions.erase(it);
}

