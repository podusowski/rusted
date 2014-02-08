#include "Cake/Diagnostics/Logger.hpp"

#include "AdministrationService.hpp"

namespace Server
{
namespace Services
{

AdministrationService::AdministrationService(Game::IPlayerContainer & playerContainer) :
    m_playerContainer(playerContainer)
{
}

void AdministrationService::handle(const Common::Messages::ReloadDatabase &, Network::IConnection &)
{
    LOG_DEBUG << "Reloading database";

    m_universeDataBaseFacade->loadObjects();

    m_playerContainer.invokeOnAllPlayers([&](Common::Game::IPlayer &, Network::IConnection & connection)
    {
        m_utils.sendVisibleObjects(*m_universe, connection);
    });
}

}
}
