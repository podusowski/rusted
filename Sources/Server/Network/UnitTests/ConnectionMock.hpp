#pragma once

#include <gmock/gmock.h>

#include "Server/Network/IConnection.hpp"

namespace Server
{
namespace Network
{

class ConnectionMock : public IConnection
{
    MOCK_METHOD1(send, void(const Common::Messages::AbstractMessage &));
    MOCK_METHOD1(addListener, void(IConnectionListener &));
    MOCK_METHOD0(getId, int());
};

}
}
