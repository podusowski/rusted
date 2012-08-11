#include <boost/shared_ptr.hpp>

#include "Common/Game/Object/Ship.hpp"
#include "BuildShip.hpp"

using namespace Server::Game::Actions;

BuildShip::BuildShip(Common::Game::Universe & universe, Server::Game::IPlayer & player) : 
    m_universe(universe),
    m_player(player)
{
}

void BuildShip::execute()
{
    boost::shared_ptr<Common::Game::Object::ObjectBase> object(new Common::Game::Object::Ship);
    Common::Game::Object::Ship & ship = dynamic_cast<Common::Game::Object::Ship &>(*object);

    ship.setOwnerId(m_player.getId());

    m_universe.add(object);
}
