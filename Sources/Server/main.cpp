#include "Cake/DependencyInjection/Registry.hpp"
#include "Cake/Configuration/Configuration.hpp"
#include "Cake/Diagnostics/Logger.hpp"

#include "Game/ObjectFactory.hpp"
#include "Network/ServerController.hpp"

void initDependencies(int argc, const char * argv[])
{
    boost::shared_ptr<Common::Game::Object::ObjectFactory> objectFactory(new Common::Game::Object::ObjectFactory());
    Cake::DependencyInjection::forInterface<Common::Game::Object::ObjectFactory>().use(objectFactory);

    boost::shared_ptr<Common::Game::IRustedTime> rustedTime(new Common::Game::RustedTime);
    Cake::DependencyInjection::forInterface<Common::Game::IRustedTime>().use(rustedTime);

    boost::shared_ptr<Cake::Configuration::Configuration> configuration(new Cake::Configuration::Configuration(argc, argv));
    Cake::DependencyInjection::forInterface<Cake::Configuration::Configuration>().use(configuration);
}

int main(int argc, const char * argv[])
{
    Cake::Diagnostics::Logger::getInstance().setAppName("Server");

    initDependencies(argc, argv);

    LOG_INFO << "This SW version was build on " << __DATE__ << " " << __TIME__;

    Server::Network::ServerController controller(argc, argv);
    return controller.start();
}

