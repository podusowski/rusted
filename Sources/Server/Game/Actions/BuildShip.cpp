#include <boost/shared_ptr.hpp>

#include "Common/Game/Object/Ship.hpp"
#include "BuildShip.hpp"

using namespace Server::Game::Actions;

BuildShip::BuildShip(
    Common::Game::Universe & universe, 
    Common::Game::IPlayer & player, 
    Server::Game::IPlayerContainer & playerContainer,
    unsigned shipClass
) : 
    m_universe(universe),
    m_player(player),
    m_playerContainer(playerContainer),
    m_shipClass(shipClass)
{
}

Common::Game::TimeValue BuildShip::start()
{
    updateCargoHold();

    return Common::Game::TimeValue(2, 0);
}

void BuildShip::finish()
{
    auto object = m_objectFactory->createShip(m_shipClass, m_player.getId());

    Common::Game::Object::Ship & ship = dynamic_cast<Common::Game::Object::Ship &>(*object);
    Common::Game::Object::Ship & focusedShip = dynamic_cast<Common::Game::Object::Ship&>(m_player.getFocusedObject());

    // FIXME: this is "invalid id", Universe will generate one when this object is added
    ship.setId(0);
    ship.setOwnerId(m_player.getId());

    Common::Game::Position focusedObjectPosition = focusedShip.getPosition();
    ship.setPosition(focusedObjectPosition);
    ship.setCourse(focusedObjectPosition + Common::Game::Position(10, 10, 0));

    m_universe.add(object);

    auto connections = m_playerContainer.getAllConnections(Common::Game::PLAYER_STATE_AUTHORIZED);
    for (auto connection: connections)
    {
        m_servicesUtils.sendObjectInfo(ship, *connection);
    }
}

void BuildShip::updateCargoHold()
{
    auto & focusedShip = dynamic_cast<Common::Game::Object::Ship&>(m_player.getFocusedObject());
    auto & shipClass = m_shipClassContainer->getById(m_shipClass);

    focusedShip.visitCargoHold([&](Common::Game::Object::CargoHold & cargoHold) -> void
    {
        unsigned requiredCarbon = shipClass.getRequiredCarbon();
        unsigned requiredHelium = shipClass.getRequiredHelium();

        cargoHold.changeCarbon(-requiredCarbon);
        cargoHold.changeHelium(-requiredHelium);

        auto & connection = m_playerContainer.getConnectionById(m_player.getId());
        m_servicesUtils.sendObjectCargoInfo(focusedShip, connection);
    });
}

