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

    MOCK_METHOD1(getBy, Common::Game::IPlayer & (Network::IConnection & connection));
    MOCK_METHOD1(getConnectionById, Network::IConnection & (int));
    MOCK_METHOD2(invokeOnPlayer, void(int, std::function<void(Common::Game::Player &, Network::IConnection &)>));
    MOCK_METHOD1(getAll, std::vector<std::shared_ptr<Common::Game::Player> > (Common::Game::PlayerState state));
    MOCK_METHOD1(getAllConnections, std::vector<Network::IConnection *> (Common::Game::PlayerState state));
    MOCK_METHOD1(getPlayerSummary, PlayerSummary(int id));
};

}
}
