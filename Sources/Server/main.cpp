#include <boost/make_shared.hpp>
#include <soci.h>

#include "Cake/DependencyInjection/Registry.hpp"
#include "Cake/Configuration/Configuration.hpp"
#include "Cake/Diagnostics/Logger.hpp"
#include "Common/Game/Object/FlightTrajectory.hpp"
#include "Common/Math/Bezier3.hpp"
#include "Common/Game/Universe.hpp"

#include "Game/ObjectFactory.hpp"
#include "Network/ServerController.hpp"
#include "DataBase/SociSessionFactory.hpp"

void initDependencies(int argc, const char * argv[])
{
    using namespace Cake::DependencyInjection;

    boost::shared_ptr<Common::Game::IRustedTime> rustedTime(new Common::Game::RustedTime);
    forInterface<Common::Game::IRustedTime>()
        .use(rustedTime);

    boost::shared_ptr<Cake::Configuration::Configuration> configuration(new Cake::Configuration::Configuration(argc, argv));
    forInterface<Cake::Configuration::Configuration>()
        .use(configuration);

    boost::shared_ptr<IFactory> sociSessionFactory(new Server::DataBase::SociSessionFactory);
    forInterface<soci::session>()
        .useFactory(sociSessionFactory);

    boost::shared_ptr<Server::Game::IShipClassContainer> shipClassContainer(new Server::Game::ShipClassContainer());
    forInterface<Server::Game::IShipClassContainer>()
        .use(shipClassContainer);

    boost::shared_ptr<Server::Game::IObjectFactory> objectFactory(new Server::Game::ObjectFactory(*shipClassContainer));
    forInterface<Server::Game::IObjectFactory>()
        .use(objectFactory);

    forInterface<Common::Game::Object::IFlightTrajectory>()
        .useFactory<GenericFactory0<Common::Game::Object::IFlightTrajectory, Common::Game::Object::FlightTrajectory> >();

    forInterface<Common::Math::ISpline3>()
        .useFactory<GenericFactory0<Common::Math::ISpline3, Common::Math::Bezier3>>();

    auto universe = boost::make_shared<Common::Game::Universe>();
    forInterface<Common::Game::Universe>().use(universe);
}

int main(int argc, const char * argv[])
{
    Cake::Diagnostics::Logger::getInstance().setAppName("Server");

    initDependencies(argc, argv);

    LOG_INFO << "This SW version was build on " << __DATE__ << " " << __TIME__;

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

