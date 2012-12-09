#include <boost/bind.hpp>

#include "ActionFactory.hpp"
#include "Attack.hpp"
#include "BuildShip.hpp"
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

    Common::Game::Object::ObjectBase * selectedObject = nullptr;

    // FIXME: you know what - but not now, when we're going to develop object deletion, it
    //        will most likely be weak_ptr anyway
    try
    {
        selectedObject = &(player.getSelectedObject());
    }
    catch (...)
    {
    }

    switch (id)
    {
        case ActionType_Attack:
        {
            assert(selectedObject);
            ret = boost::shared_ptr<IAction>(new Attack(m_playerContainer, focusedShip, *selectedObject));
            break;
        }

        case ActionType_BuildShip:
        {
            auto & player = m_playerContainer.getBy(connection);
            ret = boost::shared_ptr<IAction>(new BuildShip(m_universe, player, m_playerContainer, parameter));
            break;
        }

        default:
            throw std::runtime_error("unknown action");
    }

    if (ret)
    {
        LOG_DEBUG << "Constructed action: " << id << "/" << CAKE_DEPENDENCY_INJECTION_TYPENAME(*ret);
    }

    return ret;
}

