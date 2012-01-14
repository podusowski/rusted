#include <gtest/gtest.h>

#include "Cake/DependencyInjection/Registry.hpp"

#include "Common/Game/UnitTests/RustedTimeStub.hpp"
#include "Network/UnitTests/ConnectionMock.hpp"
#include "Game/PlayerInfo.hpp"
#include "Common/Game/Universe.hpp"
#include "Services/EntityService.hpp"

using namespace testing;

class EntityServiceTest : public Test
{
public:
    EntityServiceTest()
    {
        boost::shared_ptr<Common::Game::IRustedTime> rustedTime(new RustedTimeStub());
        Cake::DependencyInjection::forInterface<Common::Game::IRustedTime>().use(rustedTime);
    }
};

TEST_F(EntityServiceTest, EntityGetInfoResp)
{
    Client::Network::ConnectionMock connection;
    Client::Game::PlayerInfo playerInfo;
    Common::Game::Universe universe;

    Client::Services::EntityService entityService(connection, playerInfo, universe);

    Common::Messages::EntityGetInfoResp entityGetInfoResp;
    entityGetInfoResp.id = 1;
    entityGetInfoResp.player_id = 2;
    entityGetInfoResp.x = 1;
    entityGetInfoResp.y = 2;
    entityGetInfoResp.z = 3;

    EXPECT_ANY_THROW(universe.getById<Common::Game::Object::Ship>(1));

    entityService.handle(entityGetInfoResp);

    EXPECT_NO_THROW(universe.getById<Common::Game::Object::Ship>(1));
}
