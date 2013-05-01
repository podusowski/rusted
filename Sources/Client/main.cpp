#include "Cake/DependencyInjection/Registry.hpp"
#include "Cake/Diagnostics/Logger.hpp"

#include "Common/Game/RustedTime.hpp"
#include "Engine/Engine.hpp"
#include "Common/Game/Object/FlightTrajectory.hpp"
#include "Common/Math/Bezier3.hpp"

void initDependencies(int argc, const char * argv[])
{
    using namespace Cake::DependencyInjection;

    boost::shared_ptr<Common::Game::IRustedTime> rustedTime(new Common::Game::RustedTime);
    forInterface<Common::Game::IRustedTime>().use(rustedTime);

    boost::shared_ptr<Cake::Configuration::Configuration> configuration(new Cake::Configuration::Configuration(argc, argv));
    forInterface<Cake::Configuration::Configuration>().use(configuration);

    forInterface<Common::Game::Object::IFlightTrajectory>()
        .useFactory<GenericFactory0<Common::Game::Object::IFlightTrajectory, Common::Game::Object::FlightTrajectory> >();

    forInterface<Common::Math::ISpline3>()
        .useFactory<GenericFactory0<Common::Math::ISpline3, Common::Math::Bezier3>>();
}

int main(int argc, const char * argv[])
{
    Cake::Diagnostics::Logger::getInstance().setAppName("Client");

    initDependencies(argc, argv);

    LOG_INFO << "This SW version was build on " << __DATE__ << " " << __TIME__;

    Client::Engine::Engine e;
    e.start();
}

