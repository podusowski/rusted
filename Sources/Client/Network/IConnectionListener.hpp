#pragma once

#include "Common/Messages/Messages.hpp"

namespace Client
{
namespace Network
{

class IConnectionListener
{
public:
    virtual void messageReceived(Common::Messages::AbstractMessage & message) = 0;
};

}
}

