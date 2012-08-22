#include <boost/bind.hpp>

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
    Server::Game::IPlayer & player, unsigned id)
{
    aquireGlobalCooldown(player.getId());
    auto action = m_actionFactory.create(connection, player, id);
    action->execute();
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

