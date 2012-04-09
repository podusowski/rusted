#pragma once

#include "Common/Messages/Messages.hpp"

#include "IConnection.hpp"

namespace Client
{
namespace Network
{

class IConnection
{
public:
    virtual void send(Common::Messages::AbstractMessage &) = 0;
    virtual ~IConnection() {}
};

}
}
