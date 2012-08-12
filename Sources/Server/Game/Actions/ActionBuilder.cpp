#include "ActionBuilder.hpp"
#include "Attack.hpp"
#include "BuildShip.hpp"

using namespace Server::Game::Actions;

boost::shared_ptr<IAction> ActionBuilder::build(
        Server::Network::IConnection & connection,
        Server::Game::IPlayerContainer & playerContainer,
        Server::Game::IPlayer & player,
        Common::Game::Universe & universe,
        unsigned id)
{
    boost::shared_ptr<IAction> ret;

    auto & focusedObject = player.getFocusedObject();
    auto & focusedShip = dynamic_cast<Common::Game::Object::Ship&>(focusedObject);

    Common::Game::Object::ObjectBase * selectedObject = nullptr;

    // FIXME: you know what :)
    try
    {
        selectedObject = &(player.getSelectedObject());
    }
    catch (...)
    {
    }

    switch (id)
    {
        case 1:
        {
            assert(selectedObject);
            ret = boost::shared_ptr<IAction>(new Attack(playerContainer, focusedShip, *selectedObject));
            break;
        }

        case 2:
        {
            auto & player = playerContainer.getBy(connection);
            ret = boost::shared_ptr<IAction>(new BuildShip(universe, player, playerContainer));
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

