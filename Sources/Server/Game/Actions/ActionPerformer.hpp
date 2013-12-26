#pragma once

#include <utility>

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

namespace Detail
{
    struct OngoingOrCoolingAction
    {
        ActionParameters actionParameters;
        bool loop;

        bool operator < (const OngoingOrCoolingAction & other) const
        {
            // NOTE: selectedObjectId is intentionaly excluded from comparision
            return
                std::tie(
                    actionParameters.actionId,
                    actionParameters.actionParameter,
                    actionParameters.focusedObjectId) <

                std::tie(
                    other.actionParameters.actionId,
                    other.actionParameters.actionParameter,
                    other.actionParameters.focusedObjectId);
        }
    };
}

class ActionPerformer
{
public:
    ActionPerformer(IActionFactory &, Common::Game::Universe &, Server::Game::IPlayerContainer &);

    void perform(
        Server::Network::IConnection & connection,
        Common::Game::IPlayer &,
        const ActionParameters & actionParameters,
        bool loop);

private:
    void aquireGlobalCooldown(unsigned shipIp, Network::IConnection &);
    bool isGlobalCooldownActive(unsigned shipId);
    void globalCooldownExpired(unsigned shipId);

    void actionTimerExpired(unsigned internalId, unsigned playerId, unsigned objectId, unsigned actionId);

    void aquireOngoingOrCooling(
        const ActionParameters & actionParameters,
        bool loop);

    bool isActionOngoingOrCooling(unsigned shipId, unsigned actionId);
    void actionCooldownExpired(unsigned playerId, unsigned objectId, unsigned actionId);

    void actionFinished(
        std::shared_ptr<Server::Game::Actions::IAction> action,
        unsigned playerId,
        unsigned objectId,
        unsigned actionId);

    Cake::DependencyInjection::Inject<Common::Game::IRustedTime> m_time;
    IActionFactory & m_actionFactory;
    Common::Game::Universe & m_universe;
    Server::Game::IPlayerContainer & m_playerContainer;
    std::set<unsigned> m_playerGlobalCooldowns;
    std::map<unsigned, std::shared_ptr<IAction> > m_ongoingActions;
    std::set<Detail::OngoingOrCoolingAction> m_ongogingOrCoolingActions;
    Cake::Threading::Mutex m_ongoingActionsMutex;
    Common::Game::Utilities::IdGenerator m_idGenerator;
};

}
}
}

