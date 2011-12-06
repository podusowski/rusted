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
    virtual void send(Common::Messages::AbstractMessage & message) = 0;
    virtual void addListener(IConnectionListener & listener) = 0;
    virtual int getId() = 0;
};

}
}
