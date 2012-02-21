#include "Common/Game/Object/Ship.hpp"
#include "ObjectService.hpp"

using namespace Client::Services;

ObjectService::ObjectService(Common::Game::Universe & universe) :
    m_universe(universe)
{
}

void ObjectService::handle(const Common::Messages::VisibleObjects &)
{
}

void ObjectService::handle(const Common::Messages::ShipInfo & shipInfo)
{
    LOG_DEBUG << "Got ship info (id: " << shipInfo.id << ")";

    boost::shared_ptr<Common::Game::Object::ObjectBase> object(new Common::Game::Object::Ship);
    Common::Game::Object::Ship & ship = dynamic_cast<Common::Game::Object::Ship&>(*object);
    ship.setId(shipInfo.id);
    ship.setOwnerId(shipInfo.player_id);
    ship.setPosition(Common::Game::Position(shipInfo.x, shipInfo.y, shipInfo.z));

    m_universe.add(object);
}
