#include "Cake/DependencyInjection/Registry.hpp"
#include "Cake/Configuration/Configuration.hpp"
#include "Cake/Diagnostics/Logger.hpp"
#include "Server/DataBase/DataBase.hpp"

#include "Game/ObjectFactory.hpp"
#include "Network/ServerController.hpp"

void initDependencies(int argc, const char * argv[])
{
    boost::shared_ptr<Common::Game::IRustedTime> rustedTime(new Common::Game::RustedTime);
    Cake::DependencyInjection::forInterface<Common::Game::IRustedTime>().use(rustedTime);

    boost::shared_ptr<Cake::Configuration::Configuration> configuration(new Cake::Configuration::Configuration(argc, argv));
    Cake::DependencyInjection::forInterface<Cake::Configuration::Configuration>().use(configuration);

    Server::DataBase::DataBaseFactory dbFactory(*configuration);

    boost::shared_ptr<Server::DataBase::DataBase> db = dbFactory.create();
    Cake::DependencyInjection::forInterface<Server::DataBase::DataBase>().use(db);

    boost::shared_ptr<Server::Game::ObjectFactory> objectFactory(new Server::Game::ObjectFactory(*db));
    Cake::DependencyInjection::forInterface<Server::Game::ObjectFactory>().use(objectFactory);
}

int main(int argc, const char * argv[])
{
    Cake::Diagnostics::Logger::getInstance().setAppName("Server");

    initDependencies(argc, argv);

    LOG_INFO << "This SW version was build on " << __DATE__ << " " << __TIME__;

    Server::Network::ServerController controller;
    return controller.start();
}

