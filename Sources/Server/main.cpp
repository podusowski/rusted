#include <memory>
#include <soci.h>

#include "Cake/DependencyInjection/Registry.hpp"
#include "Cake/Configuration/Configuration.hpp"
#include "Cake/Diagnostics/Logger.hpp"
#include "Common/Game/Object/FlightTrajectory.hpp"
#include "Common/Math/Bezier3.hpp"
#include "Common/Game/Universe.hpp"

#include "Game/UniverseDataBaseFacade.hpp"
#include "Network/ServerController.hpp"
#include "DataBase/SociSessionFactory.hpp"

void initDependencies(int argc, const char * argv[])
{
    using namespace Cake::DependencyInjection;

    auto configuration = std::make_shared<Cake::Configuration::Configuration>(argc, argv);
    forInterface<Cake::Configuration::Configuration>()
        .use(configuration);

    forInterface<Common::Game::Universe>().use(std::make_shared<Common::Game::Universe>());

    forInterface<Common::Game::IRustedTime>().use(std::make_shared<Common::Game::RustedTime>());

    forInterface<Common::Game::Object::IFlightTrajectory>()
        .useFactory<GenericFactory0<Common::Game::Object::IFlightTrajectory, Common::Game::Object::FlightTrajectory> >();

    forInterface<Common::Math::ISpline3>()
        .useFactory<GenericFactory0<Common::Math::ISpline3, Common::Math::Bezier3>>();

    std::shared_ptr<IFactory> sociSessionFactory(new Server::DataBase::SociSessionFactory);
    forInterface<soci::session>()
        .useFactory(sociSessionFactory);

    std::shared_ptr<Server::Game::IShipClassContainer> shipClassContainer(new Server::Game::ShipClassContainer());
    forInterface<Server::Game::IShipClassContainer>()
        .use(shipClassContainer);

    std::shared_ptr<Server::Game::IUniverseDataBaseFacade> universeDbFacade(new Server::Game::UniverseDataBaseFacade(*shipClassContainer));
    forInterface<Server::Game::IUniverseDataBaseFacade>()
        .use(universeDbFacade);

}

int main(int argc, const char * argv[])
{
    Cake::Diagnostics::Logger::getInstance().setAppName("Server");
    LOG_INFO << "This SW version was build on " << __DATE__ << " " << __TIME__;

    initDependencies(argc, argv);

    try
    {
        Server::Network::ServerController controller;
        return controller.start();
    }
    catch (std::exception & ex)
    {
        LOG_ERR << "Could not initialize the server, reason: " << ex.what();
        return 1;
    }
}

