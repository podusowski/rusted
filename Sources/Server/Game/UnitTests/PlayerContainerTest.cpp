#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "Common/Game/Utilities/PasswordHash.hpp"
#include "DataBase/DataBase.hpp"
#include "UnitTests/ConnectionStub.hpp"
#include "Game/PlayerContainer.hpp"

using namespace Server::Game;
using namespace Common;

class PlayerContainerTest : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE (PlayerContainerTest);
    CPPUNIT_TEST(testAddPlayerAndReferenceByConnection);
    CPPUNIT_TEST_SUITE_END ();

public:
    void testAddPlayerAndReferenceByConnection();
};

CPPUNIT_TEST_SUITE_REGISTRATION (PlayerContainerTest);

void PlayerContainerTest::testAddPlayerAndReferenceByConnection()
{
    DataBase::DataBase db;
    DataBase::DataBaseNode & playersNode = db.getRoot().createChild("users");
    DataBase::DataBaseNode & playerNode = playersNode.createChild("1");
    playerNode.setValue("login", "someLogin");
    Common::Game::Utilities::PasswordHash hash;
    playerNode.setValue("password", hash.generate("passwordHash"));
    playerNode.setValue("id", 1);

    Server::Game::PlayerContainer container(db);
    ConnectionStub connection;
    
    Player & player1 = container.create("someLogin", "passwordHash", connection);
    Player & player2 = container.getBy(connection);

    CPPUNIT_ASSERT_EQUAL(1, player1.getId());
    CPPUNIT_ASSERT_EQUAL(1, player2.getId());
}

