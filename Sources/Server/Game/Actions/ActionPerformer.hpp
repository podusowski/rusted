#pragma once

#include "Server/Game/Actions/IAction.hpp"
#include "Server/Network/IConnection.hpp"
#include "Server/Game/IPlayerContainer.hpp"
#include "Common/Game/Universe.hpp"
#include "IActionFactory.hpp"
#include "Common/Game/IRustedTime.hpp"
#include "Cake/Threading/Mutex.hpp"

namespace Server
{
namespace Game
{
namespace Actions
{

class ActionPerformer
{
public:
    ActionPerformer(IActionFactory &, Common::Game::Universe &, Server::Game::IPlayerContainer &);

    void perform(
        Server::Network::IConnection & connection,
        Server::Game::IPlayer &,
        unsigned id);

private:
    void aquireGlobalCooldown(unsigned playerId);
    void globalCooldownExpired(unsigned playerId);

    void actionTimerExpired(unsigned internalId, unsigned playerId, unsigned objectId, unsigned actionId);

    Cake::DependencyInjection::Inject<Common::Game::IRustedTime> m_time;
    IActionFactory & m_actionFactory;
    Common::Game::Universe & m_universe;
    Server::Game::IPlayerContainer & m_playerContainer;
    std::set<unsigned> m_playerGlobalCooldowns;
    std::map<unsigned, boost::shared_ptr<IAction> > m_ongoingActions;
    Cake::Threading::Mutex m_ongoingActionsMutex;
    Common::Game::Utilities::IdGenerator m_idGenerator;
};

}
}
}

