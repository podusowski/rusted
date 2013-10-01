#pragma once

#include <string>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <netdb.h>
    #include <netinet/in.h>
#endif

namespace Cake 
{
namespace Networking
{

class DnsResolver
{
public:
    sockaddr_in resolve(const std::string & address, int port);
};

}
}
