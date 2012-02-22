#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>

#include "Cake/DependencyInjection/Registry.hpp"

#include "Game/Object/Ship.hpp"
#include "Game/Object/StaticObject.hpp"
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

TEST_F(UniverseTest, GetByType)
{
    Common::Game::Universe universe;
    boost::shared_ptr<Common::Game::Object::ObjectBase> ship(new Common::Game::Object::Ship());
    ship->setId(1);
    universe.add(ship);

    boost::shared_ptr<Common::Game::Object::ObjectBase> staticObject(new Common::Game::Object::StaticObject());
    staticObject->setId(2);
    universe.add(staticObject);

    Common::Game::Universe::Objects objects = universe.get<Common::Game::Object::Ship>();
    ASSERT_EQ(1, objects.size());
}

TEST_F(UniverseTest, GetAll)
{
    Common::Game::Universe universe;
    boost::shared_ptr<Common::Game::Object::ObjectBase> ship(new Common::Game::Object::Ship());
    ship->setId(1);
    universe.add(ship);

    boost::shared_ptr<Common::Game::Object::ObjectBase> staticObject(new Common::Game::Object::StaticObject());
    staticObject->setId(2);
    universe.add(staticObject);

    Common::Game::Universe::Objects objects = universe.getAll();
    ASSERT_EQ(2, objects.size());
}

TEST_F(UniverseTest, AddOneShipAndGetIt)
{
    Common::Game::Universe universe;
    boost::shared_ptr<Common::Game::Object::ObjectBase> ship(new Common::Game::Object::Ship());
    ship->setId(1);

    universe.add(ship);

    ASSERT_EQ(ship.get(), &(universe.getById<Common::Game::Object::Ship>(1)));
}

TEST_F(UniverseTest, GetByNonexistingId)
{
    Common::Game::Universe universe;

    ASSERT_THROW(universe.getById<Common::Game::Object::Ship>(1234), std::out_of_range);
}

TEST_F(UniverseTest, GetWrongType)
{
    Common::Game::Universe universe;
    boost::shared_ptr<Common::Game::Object::ObjectBase> ship(new Common::Game::Object::Ship());
    ship->setId(1);

    universe.add(ship);

    ASSERT_ANY_THROW(universe.getById<Common::Game::Object::StaticObject>(1));
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

class UniverseCallbackMock
{
public:
    MOCK_METHOD1(objectAdded, void(Common::Game::Object::ObjectBase &));
};

TEST_F(UniverseTest, ObjectAddedCallback)
{
    Common::Game::Universe universe;
    UniverseCallbackMock universeCallbackMock;

    EXPECT_CALL(universeCallbackMock, objectAdded(_)).Times(1);

    universe.setObjectAddedCallback(boost::bind(&UniverseCallbackMock::objectAdded, &universeCallbackMock, _1));

    // add some object
    boost::shared_ptr<Common::Game::Object::ObjectBase> ship1(new Common::Game::Object::Ship());
    ship1->setId(1);
    dynamic_cast<Common::Game::Object::OwnedObjectBase&>(*ship1).setOwnerId(1);

    universe.add(ship1);
}

TEST_F(UniverseTest, DoubleInsert)
{
    Common::Game::Universe universe;

    boost::shared_ptr<Common::Game::Object::ObjectBase> obj1(new Common::Game::Object::StaticObject());
    obj1->setId(1);

    universe.add(obj1);

    boost::shared_ptr<Common::Game::Object::ObjectBase> obj2(new Common::Game::Object::StaticObject());
    obj2->setId(1);

    EXPECT_ANY_THROW(universe.add(obj2));
}
