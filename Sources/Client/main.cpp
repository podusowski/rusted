#include "Engine/Application.hpp"
#include "Common/Logger/Logger.hpp"

int main(int argc, const char * argv[])
{
    LOG_INFO << "This SW version was build on " << __DATE__ << " " << __TIME__ << "\n";
    Client::Engine::Application application(argc, argv);
    application.start();
}
