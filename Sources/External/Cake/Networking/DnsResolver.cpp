#include <sstream>
#include <stdexcept>
#include <string.h>

#include "System/AtomicSyscall.hpp"
#include "Utils/BuildString.hpp"
#include "SocketInitialize.hpp"
#include "DnsResolver.hpp"

using namespace Cake::Networking;

sockaddr_in DnsResolver::resolve(const std::string & address, int port)
{
    SocketInitialize::tryInitialize();

    hostent * ent = gethostbyname(address.c_str());

    if (ent == 0)
    {
#ifdef _WIN32
        int error = WSAGetLastError();
#else
        int error = errno;
#endif
        throw std::runtime_error(BUILD_STRING << "can't resolve " << address << ", reason: " << error);
    }

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    memcpy(&addr.sin_addr.s_addr, ent->h_addr, ent->h_length);
    addr.sin_port = htons(port);

    return addr;
}

