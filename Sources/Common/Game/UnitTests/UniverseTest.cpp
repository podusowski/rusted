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

TEST_F(UniverseTest, AddSomeShipsAndGetByOwner)
{
    const unsigned OWNER_ID = 5;

    Common::Game::Universe universe;
    boost::shared_ptr<Common::Game::Object::ObjectBase> ship1(new Common::Game::Object::Ship());
    ship1->setId(1);
    dynamic_cast<Common::Game::Object::OwnedObjectBase&>(*ship1).setOwnerId(OWNER_ID);

    universe.add(ship1);

    boost::shared_ptr<Common::Game::Object::ObjectBase> ship2(new Common::Game::Object::Ship());
    ship2->setId(2);
    dynamic_cast<Common::Game::Object::OwnedObjectBase&>(*ship2).setOwnerId(OWNER_ID + 1);

    universe.add(ship2);

    std::vector<boost::shared_ptr<Common::Game::Object::ObjectBase> > playerObjects 
        = universe.getByOwnerId<Common::Game::Object::Ship>(OWNER_ID);

    ASSERT_EQ(1, playerObjects.size());
    ASSERT_EQ(ship1.get(), playerObjects[0].get());
}
