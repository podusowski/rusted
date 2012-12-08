#include <gtest/gtest.h>

#include "Common/Game/Utilities/PasswordHash.hpp"
#include "DataBase/DataBase.hpp"
#include "Network/UnitTests/ConnectionMock.hpp"
#include "Game/PlayerContainer.hpp"

using namespace testing;
using namespace Server::Game;
using namespace Common;

TEST(PlayerContainerTest, AddPlayerAndReferenceByConnection)
{
    Server::DataBase::DataBase db;
    Server::DataBase::DataBaseNode & playersNode = db.getRoot().createChild("users");
    auto & playerNode = playersNode.createChild("1");
    playerNode.setValue("login", "someLogin");
    Common::Game::Utilities::PasswordHash hash;
    playerNode.setValue("password", hash.generate("passwordHash"));
    playerNode.setValue("id", 1);

    Server::Game::PlayerContainer container(db);
    Server::Network::ConnectionMock connection;
    
    container.add(connection);
    int player1Id = container.authorize("someLogin", "passwordHash", connection);
    Player & player2 = container.getBy(connection);
    Server::Network::IConnection & connection2 = container.getConnectionById(player1Id);

    ASSERT_EQ(1, player1Id);
    ASSERT_EQ(1, player2.getId());
    EXPECT_EQ(&connection, &connection2);
}

