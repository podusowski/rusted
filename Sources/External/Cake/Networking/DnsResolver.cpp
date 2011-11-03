#include <sstream>
#include <stdexcept>
#include <string.h>

#include "System/AtomicSyscall.hpp"
#include "DnsResolver.hpp"

using namespace Cake::Networking;

// TODO: this still isn't thread safe...
sockaddr_in DnsResolver::resolve(const std::string & address, int port)
{
    hostent * ent;

    try
    {
        ATOMIC_SYSCALL(gethostbyname(address.c_str()), ent, == 0);
    }
    catch (std::exception & ex)
    {
        std::stringstream ss;
        ss << "couldn't resolve \"" << address << "\", reason: " << ex.what();
        throw std::runtime_error(ss.str());
    }

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    memcpy(&addr.sin_addr.s_addr, ent->h_addr, ent->h_length);
    addr.sin_port = htons(port);

    return addr;
}
