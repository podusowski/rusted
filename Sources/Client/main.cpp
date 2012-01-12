#include "Cake/DependencyInjection/Registry.hpp"
#include "Cake/Diagnostics/Logger.hpp"

#include "Common/Game/RustedTime.hpp"
#include "Engine/Application.hpp"

void initDependencies(int argc, const char * argv[])
{
    boost::shared_ptr<Common::Game::IRustedTime> rustedTime(new Common::Game::RustedTime);
    Cake::DependencyInjection::forInterface<Common::Game::IRustedTime>().use(rustedTime);

    boost::shared_ptr<Cake::Configuration::Configuration> configuration(new Cake::Configuration::Configuration(argc, argv));
    Cake::DependencyInjection::forInterface<Cake::Configuration::Configuration>().use(configuration);
}

int main(int argc, const char * argv[])
{
    initDependencies(argc, argv);

    Cake::Diagnostics::Logger::getInstance().setAppName("Client");

    LOG_INFO << "This SW version was build on " << __DATE__ << " " << __TIME__;

    Client::Engine::Application application(argc, argv);
    application.start();
}
