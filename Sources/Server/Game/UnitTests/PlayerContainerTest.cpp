#include <gtest/gtest.h>

#include "Common/Game/Utilities/PasswordHash.hpp"
#include "DataBase/DataBase.hpp"
#include "UnitTests/ConnectionStub.hpp"
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
    ConnectionStub connection;
    
    container.add(connection);
    int player1Id = container.authorize("someLogin", "passwordHash", connection);
    Player & player2 = container.getBy(connection);

    ASSERT_EQ(1, player1Id);
    ASSERT_EQ(1, player2.getId());
}

