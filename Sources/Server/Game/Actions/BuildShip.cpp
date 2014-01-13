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

Common::Game::TimeValue BuildShip::finish()
{
    auto object = m_universeDataBaseFacade->createShip(m_shipClass, m_player.getId());

    Common::Game::Object::Ship & ship = dynamic_cast<Common::Game::Object::Ship &>(*object);
    Common::Game::Object::Ship & focusedShip = dynamic_cast<Common::Game::Object::Ship&>(m_player.getFocusedObject());

    Common::Game::Position focusedObjectPosition = focusedShip.getPosition();
    ship.setPosition(focusedObjectPosition);
    ship.setCourse(focusedObjectPosition + Common::Game::Position(10, 10, 0));

    auto connections = m_playerContainer.getAllConnections(Common::Game::PLAYER_STATE_AUTHORIZED);
    for (auto connection: connections)
    {
        m_servicesUtils.sendObjectInfo(ship, *connection);
    }

    return Common::Game::TimeValue(1, 0);
}

bool BuildShip::isAbleToStart()
{
    return true;
}

std::string BuildShip::getName() const
{
    return "build";
}

std::string BuildShip::getDescription() const
{
    auto & shipClass = m_shipClassContainer->getById(m_shipClass);
    std::stringstream ss;
    ss << "build " << shipClass.getName() 
       << " (carbon: " << shipClass.getRequiredCarbon()
       << ", helium: " << shipClass.getRequiredHelium() << ")";

    return ss.str();
}

void BuildShip::updateCargoHold()
{
    auto & focusedShip = dynamic_cast<Common::Game::Object::Ship&>(m_player.getFocusedObject());
    auto & shipClass = m_shipClassContainer->getById(m_shipClass);

    focusedShip.writeCargoHold([&](Common::Game::Object::CargoHold & cargoHold) -> void
    {
        unsigned requiredCarbon = shipClass.getRequiredCarbon();
        unsigned requiredHelium = shipClass.getRequiredHelium();

        cargoHold.changeCarbon(-requiredCarbon);
        cargoHold.changeHelium(-requiredHelium);

        auto & connection = m_playerContainer.getConnectionById(m_player.getId());
        m_servicesUtils.sendObjectCargoInfo(focusedShip, connection);
    });
}

