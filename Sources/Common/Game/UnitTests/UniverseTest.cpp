#include <gtest/gtest.h>

#include <boost/shared_ptr.hpp>

#include "Cake/DependencyInjection/Registry.hpp"

#include "Game/Object/Ship.hpp"
#include "Game/Universe.hpp"

#include "Game/UnitTests/RustedTimeStub.hpp"

using namespace testing;

class UniverseTest : public Test
{
public:
    void SetUp()
    {
        Cake::DependencyInjection::clear();
        m_time.reset(new RustedTimeStub);
        Cake::DependencyInjection::forInterface<Common::Game::IRustedTime>().use(m_time);
    }

private:
    boost::shared_ptr<Common::Game::IRustedTime> m_time;
};

TEST_F(UniverseTest, AddOneShipAndGetIt)
{
    Common::Game::Universe universe;
    boost::shared_ptr<Common::Game::Object::ObjectBase> ship(new Common::Game::Object::Ship());
    ship->setId(1);

    universe.add(ship);

    ASSERT_EQ(ship.get(), &(universe.getById<Common::Game::Object::Ship>(1)));
}
