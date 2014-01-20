#include <string>
#include <iostream>
#include <vector>

#include "Cake/Networking/Protocol/CakeWriteBuffer.hpp"
#include "Cake/Networking/Socket.hpp"
#include "Common/Messages/Messages.hpp"

int main(int argc, char * argv[])
{
    std::vector<std::string> args(argv, argv + argc);

    if (args.size() < 2)
    {
        std::cerr << "no administration socket path specified";
        return 1;
    }

    auto socket = Cake::Networking::Socket::connectToUnixSocket(args[1]);

    while (std::cin.good())
    {
        std::string command;
        std::cout << "rusted> ";
        std::getline(std::cin, command);

        try
        {
            auto message = Common::Messages::MessageFactory::create(command);
            std::cout << "sending " << *message << "\n";

            Cake::Networking::Protocol::CakeWriteBuffer buffer(*socket);
            message->serialize(buffer);
        }
        catch (const std::exception & ex)
        {
            std::cout << ex.what() << "\n";
        }
    }
}

