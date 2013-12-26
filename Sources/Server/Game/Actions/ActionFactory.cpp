#include <boost/bind.hpp>

#include "ActionFactory.hpp"
#include "Attack.hpp"
#include "BuildShip.hpp"
#include "Gather.hpp"
#include "Transfer.hpp"
#include "ActionType.hpp"

using namespace Server::Game::Actions;

ActionFactory::ActionFactory(Common::Game::Universe & universe, Server::Game::IPlayerContainer & playerContainer) :
    m_universe(universe),
    m_playerContainer(playerContainer)
{
}

std::shared_ptr<IAction> ActionFactory::create(
    Server::Network::IConnection & connection,
    Common::Game::IPlayer & player,
    const ActionParameters & actionParameters)
{
    std::shared_ptr<IAction> ret;

    auto & focusedObject = player.getFocusedObject();
    auto & focusedShip = dynamic_cast<Common::Game::Object::Ship&>(focusedObject);

    switch (actionParameters.actionId)
    {
        case ActionType_Attack:
            ret = std::shared_ptr<IAction>(new Attack(m_universe, m_playerContainer, actionParameters));
            break;

        case ActionType_BuildShip:
            ret = std::shared_ptr<IAction>(new BuildShip(m_universe, player, m_playerContainer, actionParameters.actionParameter));
            break;

        case ActionType_Gather:
            ret = std::shared_ptr<IAction>(new Gather(connection, m_playerContainer, player));
            break;

        case ActionType_Transfer:
            ret = std::shared_ptr<IAction>(new Transfer(connection, m_playerContainer, player));
            break;

        default:
            throw std::runtime_error("unknown action");
    }

    if (ret)
    {
        LOG_DEBUG << "Constructed action: " << actionParameters.actionId << "/" << CAKE_DEPENDENCY_INJECTION_TYPENAME(*ret);
    }

    return ret;
}

