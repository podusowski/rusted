#include <boost/shared_ptr.hpp>

#include "Common/Game/Object/Ship.hpp"
#include "BuildShip.hpp"

using namespace Server::Game::Actions;

BuildShip::BuildShip(Common::Game::Universe & universe, Server::Game::IPlayer & player, Server::Game::IPlayerContainer & playerContainer) : 
    m_universe(universe),
    m_player(player),
    m_playerContainer(playerContainer)
{
}

void BuildShip::execute()
{
    boost::shared_ptr<Common::Game::Object::ObjectBase> object(new Common::Game::Object::Ship);
    Common::Game::Object::Ship & ship = dynamic_cast<Common::Game::Object::Ship &>(*object);
    Common::Game::Object::Ship & focusedShip = dynamic_cast<Common::Game::Object::Ship&>(m_player.getFocusedObject());

    // FIXME: this is "invalid id", Universe will generate one when this object is added
    ship.setId(0);
    ship.setOwnerId(m_player.getId());

    Common::Game::Position focusedObjectPosition = focusedShip.getPosition();
    ship.setPosition(focusedObjectPosition);
    ship.setCourse(focusedObjectPosition + Common::Game::Position(10, 10, 0));

    auto connections = m_playerContainer.getAllConnections(Server::Game::PLAYER_STATE_AUTHORIZED);
    for (auto connection: connections)
    {
        m_servicesUtils.sendObjectInfo(ship, *connection);
    }

    m_universe.add(object);
}

