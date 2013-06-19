#include <boost/bind.hpp>

#include "Common/Game/Object/Ship.hpp"
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
    Common::Game::IPlayer & player, unsigned id, unsigned parameter, bool loop)
{
    // TODO: lock
    auto focusedShipId = player.getFocusedObject().getId();

    aquireGlobalCooldown(focusedShipId);

    auto action = m_actionFactory.create(connection, player, id, parameter);
    auto timeToFinish = action->start();

    auto & selectedObject = player.getFocusedObject();

    Common::Messages::ActionStarted actionStarted;
    actionStarted.actionId = id;
    actionStarted.objectId = selectedObject.getId();
    connection.send(actionStarted);

    if (timeToFinish == Common::Game::TimeValue(0, 0))
    {
        LOG_DEBUG << "Action doesn't have execution time and will be finished immidiately";

        actionFinished(action, player.getId(), selectedObject.getId(), id);
    }
    else
    {
        LOG_DEBUG << "Action started, will be finished after " << timeToFinish << "secs, loop: " << loop;

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

void ActionPerformer::aquireGlobalCooldown(unsigned shipId)
{
    auto ret = m_playerGlobalCooldowns.insert(shipId);
    if (ret.second)
    {
        LOG_DEBUG << "Global cooldown activated on ship: " << shipId;
        m_time->createTimer(Common::Game::TimeValue(1, 0), boost::bind(&ActionPerformer::globalCooldownExpired, this, shipId));
    }
    else
    {
        LOG_WARN << "Global cooldown is already active on ship: " << shipId << ", this might suggest hack attempt";
        throw std::runtime_error("global cooldown is active");
    }
}

void ActionPerformer::globalCooldownExpired(unsigned shipId)
{
    LOG_DEBUG << "Global cooldown expired for ship: " << shipId;

    auto & ship = m_universe.getById<Common::Game::Object::Ship>(shipId);
    auto & connection = m_playerContainer.getConnectionById(ship.getOwnerId());
    Common::Messages::GlobalCooldownExpired globalCooldownExpired;
    connection.send(globalCooldownExpired);

    size_t elementsErased = m_playerGlobalCooldowns.erase(shipId);
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

    // TODO: shrink critical section
    boost::shared_ptr<IAction> action = it->second;

    m_ongoingActions.erase(it);

    actionFinished(action, playerId, objectId, actionId);
}

void ActionPerformer::actionFinished(
    boost::shared_ptr<Server::Game::Actions::IAction> action,
    unsigned playerId, unsigned objectId, unsigned actionId)
{
    action->finish();

    Common::Messages::ActionFinished actionFinished;
    actionFinished.objectId = objectId;
    actionFinished.actionId = actionId;

    auto & connection = m_playerContainer.getConnectionById(playerId);
    connection.send(actionFinished);
}

