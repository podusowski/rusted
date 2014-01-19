#pragma once

#include "Cake/DependencyInjection/Inject.hpp"

#include "Game/IUniverseDataBaseFacade.hpp"
#include "Services/AbstractService.hpp"

namespace Server
{
namespace Services
{

class AdministrationService : public Server::AbstractService<AdministrationService>
{
public:
    void handle(const Common::Messages::RealoadDatabase &, Network::IConnection &);

    void handle(const Common::Messages::AbstractMessage &, Network::IConnection &) {}

private:
    Cake::DependencyInjection::Inject<Game::IUniverseDataBaseFacade> m_universeDataBaseFacade;
};

}
}

