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

void ActionPerformer::perform(
    Server::Network::IConnection & connection,
    Common::Game::IPlayer & player,
    const ActionParameters & actionParameters,
    bool loop)
{
    if (isGlobalCooldownActive(actionParameters.focusedObjectId.get()))
    {
        throw std::runtime_error("global cooldown is active for ship");
    }

    if (isActionOngoingOrCooling(actionParameters.focusedObjectId.get(), actionParameters.actionId))
    {
        throw std::runtime_error("action cooldown active or action ongoing");
    }

    auto action = m_actionFactory.create(connection, player, actionParameters);

    if (!action->isAbleToStart())
    {
        throw std::runtime_error("action can't be started at the moment");
    }

    aquireGlobalCooldown(actionParameters.focusedObjectId.get(), connection);
    aquireOngoingOrCooling(actionParameters, loop);

    Common::Messages::ActionStarted actionStarted;
    actionStarted.actionId = actionParameters.actionId;
    actionStarted.objectId = actionParameters.focusedObjectId.get();
    connection.send(actionStarted);

    Common::Game::TimeValue timeToFinish;
    try
    {
        timeToFinish = action->start();
    }
    catch (const std::exception & ex)
    {
        actionCooldownExpired(player.getId(), actionParameters.focusedObjectId.get(), actionParameters.actionId);
        LOG_ERR << "Can't start action, reason: " << ex.what();
    }

    if (timeToFinish == Common::Game::TimeValue(0, 0))
    {
        LOG_DEBUG << "Action doesn't have execution time and will be finished immidiately";

        actionFinished(action, player.getId(), actionParameters.focusedObjectId.get(), actionParameters.actionId);
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
        m_time->createTimer(
            timeToFinish,
            boost::bind(
                &ActionPerformer::actionTimerExpired, this,
                internalId, player.getId(), actionParameters.focusedObjectId.get(), actionParameters.actionId));
    }
}

void ActionPerformer::aquireGlobalCooldown(unsigned shipId, Network::IConnection & connection)
{
    auto ret = m_playerGlobalCooldowns.insert(shipId);
    if (ret.second)
    {
        LOG_DEBUG << "Global cooldown activated on ship: " << shipId;
        m_time->createTimer(Common::Game::TimeValue(1, 0), boost::bind(&ActionPerformer::globalCooldownExpired, this, shipId));

        Common::Messages::GlobalCooldownActivated globalCooldownActivated;
        connection.send(globalCooldownActivated);
    }
    else
    {
        LOG_WARN << "Global cooldown is already active on ship: " << shipId << ", this might suggest hack attempt";
        throw std::runtime_error("global cooldown is active");
    }
}

bool ActionPerformer::isGlobalCooldownActive(unsigned shipId)
{
    return m_playerGlobalCooldowns.find(shipId) != m_playerGlobalCooldowns.end();
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
    std::shared_ptr<IAction> action = it->second;

    m_ongoingActions.erase(it);

    actionFinished(action, playerId, objectId, actionId);
}

void ActionPerformer::aquireOngoingOrCooling(
    const ActionParameters & actionParameters,
    bool loop)
{
    Detail::OngoingOrCoolingAction ongoingOrCoolingAction = {
        actionParameters,
        loop
    };

    auto ret = m_ongogingOrCoolingActions.insert(ongoingOrCoolingAction);

    if (ret.second)
    {
        LOG_DEBUG << "Action added as ongoing or waiting for cooldown: " << actionParameters;
    }
    else
    {
        LOG_WARN << "Action is ongoing or waiting for cooldown: " << actionParameters;
        throw std::runtime_error("action is ongoing or waiting for cooldown");
    }
}

bool ActionPerformer::isActionOngoingOrCooling(unsigned shipId, unsigned actionId)
{
    for (auto ongoingOrCoolingAction : m_ongogingOrCoolingActions)
    {
        if (ongoingOrCoolingAction.actionParameters.actionId == actionId &&
            ongoingOrCoolingAction.actionParameters.focusedObjectId == shipId)
        {
            return true;
        }
    }
    return false;
}

void ActionPerformer::actionCooldownExpired(unsigned playerId, unsigned objectId, unsigned actionId)
{
    LOG_DEBUG << "Action cooldown finished for player:" << playerId << ", object:" << objectId;

    Common::Messages::ActionCooldownExpired actionCooldownExpired;
    auto & connection = m_playerContainer.getConnectionById(playerId);
    connection.send(actionCooldownExpired);

    auto itToDelete = std::find_if(
        m_ongogingOrCoolingActions.begin(),
        m_ongogingOrCoolingActions.end(),
        [&](const Detail::OngoingOrCoolingAction & a) -> bool
        {
            return a.actionParameters.actionId == actionId &&
                   a.actionParameters.focusedObjectId == objectId;
        });

    if (itToDelete != m_ongogingOrCoolingActions.end())
    {
        // save the data and delete from container
        auto ongoingOrCoolingAction = *itToDelete;
        m_ongogingOrCoolingActions.erase(itToDelete);

        if (itToDelete->loop)
        {
            LOG_DEBUG << "Loop is active, restarting action";

            m_playerContainer.invokeOnPlayer(connection, [&](Common::Game::IPlayer & player)
            {
                Common::Game::IPlayer::Id playerId(player.getId());

                ActionParameters actionParameters(
                    playerId,
                    ongoingOrCoolingAction.actionParameters.actionId,
                    ongoingOrCoolingAction.actionParameters.actionParameter,
                    ongoingOrCoolingAction.actionParameters.focusedObjectId,
                    ongoingOrCoolingAction.actionParameters.selectedObjectId);

                perform(
                    connection,
                    player,
                    actionParameters,
                    true);
            });
        }
    }
    else
    {
        throw std::runtime_error("action cooldown expired but no such cooldown registered");
    }
}

void ActionPerformer::actionFinished(
    std::shared_ptr<Server::Game::Actions::IAction> action,
    unsigned playerId, unsigned objectId, unsigned actionId)
{
    Common::Game::TimeValue actionCooldown;

    try
    {
        actionCooldown = action->finish();
    }
    catch (std::exception & ex)
    {
        LOG_WARN << "Action is not completed successfully, reason: " << ex.what();
        throw;
    }

    Common::Messages::ActionFinished actionFinished;
    actionFinished.objectId = objectId;
    actionFinished.actionId = actionId;

    auto & connection = m_playerContainer.getConnectionById(playerId);
    connection.send(actionFinished);

    if (actionCooldown != Common::Game::TimeValue())
    {
        m_time->createTimer(actionCooldown, boost::bind(
            &ActionPerformer::actionCooldownExpired, this,
            playerId, objectId, actionId));
    }
    else
    {
        actionCooldownExpired(playerId, objectId, actionId);
    }
}

