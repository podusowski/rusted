#ifdef _WIN32
    #include <winsock2.h>
    #include <windows.h>
#else
    #include <sys/socket.h>
    #include <sys/un.h>
    #include <netinet/in.h>
    #include <netdb.h>
#endif

#include "Utils/BuildString.hpp"
#include "Error.hpp"

using namespace Cake::Networking::Detail;

std::string Error::lastError()
{
#ifdef _WIN32
    int error = WSAGetLastError();
    return BUILD_STRING << "WSA error: " << error;
#else
    return strerror(errno);
#endif
}
