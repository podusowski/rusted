#include "Common/Logger/Logger.hpp"
#include "Network/ServerController.hpp"

int main(int argc, const char * argv[])
{
    LOG_INFO << "This SW version was build on " << __DATE__ << " " << __TIME__ << "\n";
    Server::ServerController controller(argc, argv);
    return controller.start();
}
