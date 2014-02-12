#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>

#include "Common/UnitTests/AbstractTest.hpp"

#include "Game/Object/Ship.hpp"
#include "Game/Object/UnitTests/ShipMock.hpp"
#include "Game/Object/Asteroid.hpp"
#include "Game/Object/FlightTrajectory.hpp"

#include "Game/Universe.hpp"

using namespace testing;
using namespace Common::Game;

class UniverseTest : public Common::AbstractTest
{
};

TEST_F(UniverseTest, GetByType)
{
    Common::Game::Universe universe;
    std::shared_ptr<Common::Game::Object::ObjectBase> ship(new Common::Game::Object::Ship());
    ship->setId(1);
    universe.add(ship);

    std::shared_ptr<Common::Game::Object::ObjectBase> staticObject(new Common::Game::Object::Asteroid());
    staticObject->setId(2);
    universe.add(staticObject);

    Common::Game::Universe::Objects objects = universe.get<Common::Game::Object::Ship>();
    ASSERT_EQ(1u, objects.size());
}

TEST_F(UniverseTest, GetAll)
{
    Common::Game::Universe universe;
    std::shared_ptr<Common::Game::Object::ObjectBase> ship(new Common::Game::Object::Ship());
    ship->setId(1);
    universe.add(ship);

    std::shared_ptr<Common::Game::Object::ObjectBase> staticObject(new Common::Game::Object::Asteroid());
    staticObject->setId(2);
    universe.add(staticObject);

    Common::Game::Universe::Objects objects = universe.getAll();
    ASSERT_EQ(2u, objects.size());
}

TEST_F(UniverseTest, AddOneShipAndGetIt)
{
    Common::Game::Universe universe;
    std::shared_ptr<Common::Game::Object::ObjectBase> ship(new Common::Game::Object::Ship());
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
    std::shared_ptr<Common::Game::Object::ObjectBase> ship(new Common::Game::Object::Ship());
    ship->setId(1);

    universe.add(ship);

    ASSERT_ANY_THROW(universe.getById<Common::Game::Object::Asteroid>(1));
}

TEST_F(UniverseTest, AddSomeShipsAndGetByOwner)
{
    const unsigned OWNER_ID = 5;

    Common::Game::Universe universe;
    std::shared_ptr<Common::Game::Object::ObjectBase> ship1(new Common::Game::Object::Ship());
    ship1->setId(1);
    dynamic_cast<Common::Game::Object::OwnedObjectBase&>(*ship1).setOwnerId(OWNER_ID);

    universe.add(ship1);

    std::shared_ptr<Common::Game::Object::ObjectBase> ship2(new Common::Game::Object::Ship());
    ship2->setId(2);
    dynamic_cast<Common::Game::Object::OwnedObjectBase&>(*ship2).setOwnerId(OWNER_ID + 1);

    universe.add(ship2);

    std::vector<std::shared_ptr<Common::Game::Object::ObjectBase> > playerObjects 
        = universe.getByOwnerId<Common::Game::Object::Ship>(OWNER_ID);

    ASSERT_EQ(1u, playerObjects.size());
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

    universe.addObjectAddedCallback(boost::bind(&UniverseCallbackMock::objectAdded, &universeCallbackMock, _1));

    // add some object
    std::shared_ptr<Common::Game::Object::ObjectBase> ship1(new Common::Game::Object::Ship());
    ship1->setId(1);
    dynamic_cast<Common::Game::Object::OwnedObjectBase&>(*ship1).setOwnerId(1);

    universe.add(ship1);
}

TEST_F(UniverseTest, DoubleInsert)
{
    Common::Game::Universe universe;

    std::shared_ptr<Common::Game::Object::ObjectBase> obj1(new Common::Game::Object::Asteroid());
    obj1->setId(1);

    universe.add(obj1);

    std::shared_ptr<Common::Game::Object::ObjectBase> obj2(new Common::Game::Object::Asteroid());
    obj2->setId(1);

    EXPECT_ANY_THROW(universe.add(obj2));
}

// TODO: deprecated
TEST_F(UniverseTest, GetByOwnerId)
{
    Common::Game::Universe universe;
    std::shared_ptr<Common::Game::Object::ObjectBase> ship(new Common::Game::Object::Ship());
    ship->setId(1);
    dynamic_cast<Common::Game::Object::Ship&>(*ship).setOwnerId(2);
    universe.add(ship);

    std::shared_ptr<Common::Game::Object::ObjectBase> staticObject(new Common::Game::Object::Asteroid());
    staticObject->setId(2);
    universe.add(staticObject);

    auto objects = universe.getByOwnerId<Common::Game::Object::Ship>(2);
    ASSERT_EQ(1u, objects.size());
}

TEST_F(UniverseTest, Has)
{
    Common::Game::Universe universe;
    std::shared_ptr<Common::Game::Object::ObjectBase> ship(new Common::Game::Object::Ship());
    ship->setId(1);
    dynamic_cast<Common::Game::Object::Ship&>(*ship).setOwnerId(2);
    universe.add(ship);

    ASSERT_TRUE(universe.has(1));
    ASSERT_FALSE(universe.has(2));
}

TEST_F(UniverseTest, IsOwnedBy)
{
    Common::Game::Universe universe;

    std::shared_ptr<Common::Game::Object::ObjectBase> ship(new Common::Game::Object::Ship());
    ship->setId(1);
    dynamic_cast<Common::Game::Object::Ship&>(*ship).setOwnerId(2);
    universe.add(ship);

    std::shared_ptr<Common::Game::Object::ObjectBase> ship2(new Common::Game::Object::Ship());
    ship2->setId(2);
    dynamic_cast<Common::Game::Object::Ship&>(*ship2).setOwnerId(3);
    universe.add(ship2);

    std::shared_ptr<Common::Game::Object::ObjectBase> staticObject(new Common::Game::Object::Asteroid());
    staticObject->setId(3);
    universe.add(staticObject);

    EXPECT_TRUE(universe.isOwnedBy(1, 2));
    EXPECT_TRUE(universe.isOwnedBy(2, 3));
    EXPECT_FALSE(universe.isOwnedBy(1, 3)); // different owner
    EXPECT_FALSE(universe.isOwnedBy(99, 3)); // invalid object
    EXPECT_FALSE(universe.isOwnedBy(3, 1)); // not a ship
}

struct ObjectsVisitorCallbackMock
{
    MOCK_METHOD1(call, void(Object::ObjectBase &));

    void operator()(Object::ObjectBase & o)
    {
        call(o);
    }
};

TEST_F(UniverseTest, ObjectsInProximity)
{
    const Position CENTER{ 0, 0, 0 };

    Universe universe;

    auto ship1 = std::make_shared<Common::Game::Object::ShipMock>();
    EXPECT_CALL(*ship1, getPosition()).WillOnce(Return(Position(0, 0, 0)));
    EXPECT_CALL(*ship1, getId()).Times(AtLeast(1)).WillRepeatedly(Return(1));
    universe.add(ship1);

    auto ship2 = std::make_shared<Common::Game::Object::ShipMock>();
    EXPECT_CALL(*ship2, getPosition()).WillOnce(Return(Position(0, 100, 0)));
    EXPECT_CALL(*ship2, getId()).Times(AtLeast(1)).WillRepeatedly(Return(2));
    universe.add(ship2);

    auto ship3 = std::make_shared<Common::Game::Object::ShipMock>();
    EXPECT_CALL(*ship3, getPosition()).WillOnce(Return(Position(0, 1000, 0)));
    EXPECT_CALL(*ship3, getId()).Times(AtLeast(1)).WillRepeatedly(Return(3));
    universe.add(ship3);

    ObjectsVisitorCallbackMock callbackMock;
    EXPECT_CALL(callbackMock, call(Ref(*ship1)));
    EXPECT_CALL(callbackMock, call(Ref(*ship2)));

    universe.objectsInProximity(CENTER, 500, std::ref(callbackMock));
}

TEST_F(UniverseTest, ObjectOwnedByPlayer)
{
    const int PLAYER_ID = 2;

    Common::Game::Universe universe;

    auto ship = std::make_shared<Common::Game::Object::Ship>();
    ship->setId(1);
    ship->setOwnerId(PLAYER_ID);
    universe.add(ship);

    auto staticObject = std::make_shared<Common::Game::Object::Asteroid>();
    staticObject->setId(2);
    universe.add(staticObject);

    StrictMock<ObjectsVisitorCallbackMock> callbackMock;
    EXPECT_CALL(callbackMock, call(Ref(*ship)));

    universe.objectsOwnedByPlayer(PLAYER_ID, std::ref(callbackMock));
}

