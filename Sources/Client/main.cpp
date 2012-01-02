#include "Cake/DependencyInjection/Registry.hpp"

#include "Common/Game/RustedTime.hpp"
#include "Common/Logger/Logger.hpp"
#include "Engine/Application.hpp"

void initDependencies()
{
    boost::shared_ptr<Common::Game::IRustedTime> rustedTime(new Common::Game::RustedTime);
    Cake::DependencyInjection::forInterface<Common::Game::IRustedTime>().use(rustedTime);
}

int main(int argc, const char * argv[])
{
    LOG_INFO << "This SW version was build on " << __DATE__ << " " << __TIME__ << "\n";

    initDependencies();

    Client::Engine::Application application(argc, argv);
    application.start();
}
