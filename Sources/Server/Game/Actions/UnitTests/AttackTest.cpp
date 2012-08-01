#include <gtest/gtest.h>

#include "Common/Game/Object/UnitTests/ShipMock.hpp"
#include "Common/Game/UnitTests/RustedTimeStub.hpp"
#include "Game/Actions/Attack.hpp"

using namespace testing;

class AttackTest : public Test
{
public:
    void SetUp()
    {
        Cake::DependencyInjection::clear();
        boost::shared_ptr<Common::Game::IRustedTime> time(new RustedTimeStub);
        Cake::DependencyInjection::forInterface<Common::Game::IRustedTime>().use(time);

        ON_CALL(dynamic_cast<RustedTimeStub&>(*time), getCurrentTime()).WillByDefault(Return(Common::Game::TimeValue()));
    }
};

TEST_F(AttackTest, JustAttack)
{
    Common::Game::Object::ShipMock ship1;
    Server::Game::Actions::Attack attack;

    // doesn't matter if this is called, might be cached or something
    // the thing is what to return if it's called
    ON_CALL(ship1, getIntegrity()).WillByDefault(Return(100));

    EXPECT_CALL(ship1, setIntegrity(90)).Times(1);

    attack.execute(ship1);
}
