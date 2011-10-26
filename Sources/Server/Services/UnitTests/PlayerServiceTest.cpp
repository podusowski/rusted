#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "Common/DataBase/DataBase.hpp"
#include "Common/Game/EntityContainer.hpp"
#include "Services/PlayerService.hpp"
#include "UnitTests/ConnectionStub.hpp"
#include "Common/Game/UnitTests/RustedTimeStub.hpp"

using namespace Server;
using namespace Server::Services;
using namespace Common;

class PlayerServiceTest : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE (PlayerServiceTest);
    CPPUNIT_TEST (testEntitiesStatus);
    CPPUNIT_TEST_SUITE_END ();

protected:
    void testEntitiesStatus();

};

CPPUNIT_TEST_SUITE_REGISTRATION (PlayerServiceTest);

const int PLAYER_ID = 1;
const int ENTITY_ID = 100;

void PlayerServiceTest::testEntitiesStatus()
{
    // create player and entity
    Common::DataBase::DataBase db;
    RustedTimeStub time;
    Common::Game::EntityContainer entities(db, time);
    PlayerService player(entities);
    Common::Game::Entity & entity = entities.create(PLAYER_ID);

    ConnectionStub connection;

    Common::Messages::PlayerEntitiesStatusReq playerEntitiesStatusReq;
    player.messageReceived(connection, playerEntitiesStatusReq);

    CPPUNIT_ASSERT_EQUAL(size_t(1), connection.getReceivedCount());

    std::auto_ptr<Common::Messages::PlayerEntitiesStatusResp> playerEntitiesStatusResp =
            connection.getReceivedMessage<Common::Messages::PlayerEntitiesStatusResp>(0);

    CPPUNIT_ASSERT_EQUAL(size_t(1), playerEntitiesStatusResp->entities.size());
    CPPUNIT_ASSERT_EQUAL(entity.getId(), playerEntitiesStatusResp->entities.at(0).get<0>());
}
