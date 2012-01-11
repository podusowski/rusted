#include "Cake/DependencyInjection/Registry.hpp"

#include "Common/Logger/Logger.hpp"
#include "Network/ServerController.hpp"

void initDependencies()
{
    boost::shared_ptr<Common::Game::Object::ObjectFactory> objectFactory(new Common::Game::Object::ObjectFactory());
    Cake::DependencyInjection::forInterface<Common::Game::Object::ObjectFactory>().use(objectFactory);

    boost::shared_ptr<Common::Game::IRustedTime> rustedTime(new Common::Game::RustedTime);
    Cake::DependencyInjection::forInterface<Common::Game::IRustedTime>().use(rustedTime);
}

int main(int argc, const char * argv[])
{
    LOG_INFO << "This SW version was build on " << __DATE__ << " " << __TIME__ << "\n";

    initDependencies();

    Server::Network::ServerController controller(argc, argv);
    return controller.start();
}

