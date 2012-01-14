#pragma once

#include <gmock/gmock.h>

#include "Network/IConnection.hpp"

namespace Client
{
namespace Network
{

class ConnectionMock : public IConnection
{
    MOCK_METHOD1(send, void(Common::Messages::AbstractMessage &));
};

}
}
