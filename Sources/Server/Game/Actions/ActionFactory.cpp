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

boost::shared_ptr<IAction> ActionFactory::create(
        Server::Network::IConnection & connection,
        Common::Game::IPlayer & player,
        unsigned id,
        unsigned parameter)
{
    boost::shared_ptr<IAction> ret;

    auto & focusedObject = player.getFocusedObject();
    auto & focusedShip = dynamic_cast<Common::Game::Object::Ship&>(focusedObject);

    switch (id)
    {
        case ActionType_Attack:
            ret = boost::shared_ptr<IAction>(new Attack(m_playerContainer, focusedShip, player.getSelectedObject()));
            break;

        case ActionType_BuildShip:
            ret = boost::shared_ptr<IAction>(new BuildShip(m_universe, player, m_playerContainer, parameter));
            break;

        case ActionType_Gather:
            ret = boost::shared_ptr<IAction>(new Gather(connection, m_playerContainer, player));
            break;

        case ActionType_Transfer:
            ret = boost::shared_ptr<IAction>(new Transfer(connection, m_playerContainer, player));
            break;

        default:
            throw std::runtime_error("unknown action");
    }

    if (ret)
    {
        LOG_DEBUG << "Constructed action: " << id << "/" << CAKE_DEPENDENCY_INJECTION_TYPENAME(*ret);
    }

    return ret;
}

ActionDescription ActionFactory::getActionDescription(unsigned id, unsigned parameter)
{
    ActionDescription ret;
    ret.id = id;
    ret.parameter = parameter;

    switch (id)
    {
        case ActionType_Attack:
        {
            ret.name = "attack";
            ret.description = "attack selected ship with\nsome default gun";
            break;
        }
        case ActionType_BuildShip:
        {
            auto & shipClass = m_shipClassContainer->getById(parameter);
            std::stringstream ss;
            ss << "build " << shipClass.getName() 
               << " (carbon: " << shipClass.getRequiredCarbon()
               << ", helium: " << shipClass.getRequiredHelium() << ")";

            ret.name = "build";
            ret.description = ss.str();
            break;
        }
        case ActionType_Gather:
        {
            ret.name = "gather";
            ret.description = "gather carbon and helium\nfrom selected asteroid";
            break;
        }
        case ActionType_Transfer:
        {
            ret.name = "transfer";
            ret.description = "transfer 10C and 10H to selected ship";
            break;
        }
        default:
        {
            std::stringstream ss;
            ss << "unknown action: " << id;
            throw std::runtime_error(ss.str());
        }
    }

    return ret;
}

