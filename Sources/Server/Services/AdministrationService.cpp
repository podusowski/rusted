#include "Cake/Diagnostics/Logger.hpp"

#include "AdministrationService.hpp"

namespace Server
{
namespace Services
{

void AdministrationService::handle(const Common::Messages::RealoadDatabase &, Network::IConnection &)
{
    LOG_DEBUG << "Reloading database";

    m_universeDataBaseFacade->loadObjects();
}

}
}
