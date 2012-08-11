#include <boost/shared_ptr.hpp>

#include "Common/Game/Object/Ship.hpp"
#include "BuildShip.hpp"

using namespace Server::Game::Actions;

BuildShip::BuildShip(Common::Game::Universe & universe) : m_universe(universe)
{
}

void BuildShip::execute()
{
    boost::shared_ptr<Common::Game::Object::ObjectBase> ship(new Common::Game::Object::Ship);
    m_universe.add(ship);
}
