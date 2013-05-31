#pragma once

#include <Messages/Messages.hpp>
#include <Network/IConnectionListener.hpp>

namespace Server
{
namespace Network
{

class IConnectionListener;

class IConnection
{
public:
    virtual ~IConnection() {}
    virtual void send(const Common::Messages::AbstractMessage & message) = 0;
    virtual void addListener(IConnectionListener & listener) = 0;
};

}
}

