#pragma once

#include <string>
#include <netdb.h>
#include <netinet/in.h>

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
