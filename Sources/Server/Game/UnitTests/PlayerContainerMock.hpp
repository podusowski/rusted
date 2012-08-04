#pragma once

#include <gmock/gmock.h>

#include "Server/Game/IPlayerContainer.hpp"

namespace Server
{
namespace Game
{

class PlayerContainerMock : public IPlayerContainer
{
public:
    MOCK_METHOD3(authorize, int(const std::string & login,
                                const std::string & password, 
                                Network::IConnection & connection));

    MOCK_METHOD1(add, void(Network::IConnection &));
    MOCK_METHOD1(remove, void(Network::IConnection &));

    MOCK_METHOD1(getBy, Server::Game::Player & (Network::IConnection & connection));
    MOCK_METHOD1(getAll, std::vector<boost::shared_ptr<Player> > (PlayerState state));
    MOCK_METHOD1(getAllConnections, std::vector<Network::IConnection *> (PlayerState state));
};

}
}
