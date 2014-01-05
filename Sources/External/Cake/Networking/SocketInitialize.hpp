#pragma once

namespace Cake
{
namespace Networking
{

class SocketInitialize
{
public:
    static void tryInitialize();

private:
    static bool s_initialized;
};

}
}

