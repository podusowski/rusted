#include <string>
#include <iostream>
#include <vector>

#include "Common/Messages/Messages.hpp"
#include "Cake/Networking/Socket.hpp"

int main(int argc, char * argv[])
{
    std::vector<std::string> args(argv, argv + argc);

    if (args.size() < 2)
    {
        std::cerr << "no administration socket path specified";
        return 1;
    }

    auto socket = Cake::Networking::Socket::connectToUnixSocket(args[1]);

    while (true)
    {
        std::string command;
        std::cout << "rusted> ";
        std::getline(std::cin, command);
    }
}

