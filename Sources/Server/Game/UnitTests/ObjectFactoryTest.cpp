#include <gtest/gtest.h>

#include "Server/UnitTests/AbstractTest.hpp"

#include "Game/ObjectFactory.hpp"
#include "Common/Game/Object/Ship.hpp"
#include "Common/Game/Object/Asteroid.hpp"
#include "DataBase/DataBaseNode.hpp"
#include "DataBase/DataBase.hpp"
#include "Game/UnitTests/ShipClassContainerMock.hpp"
#include "Game/UnitTests/ShipClassMock.hpp"

using namespace testing;

class ObjectFactoryTest : public Server::AbstractTest
{
public:
    void SetUp()
    {
        ON_CALL(getTimeMock(), getCurrentTime()).WillByDefault(Return(Common::Game::TimeValue()));
    }
};

TEST_F(ObjectFactoryTest, CreateShipFromDbNode)
{
    Server::DataBase::DataBase db;

    auto & node = db.getRoot().createChild("object");

    node.setValue("type", "Ship");
    node.setValue("id", 1);
    node.setValue("owner", 2);
    node.setValue("x", 3);
    node.setValue("y", 4);
    node.setValue("z", 5);
    node.setValue("integrity", 100);
    node.setValue("class", 1);

    Server::Game::ShipClassMock shipClass;
    EXPECT_CALL(shipClass, applyTo(_)).Times(1);

    Server::Game::ShipClassContainerMock shipClassContainer;
    EXPECT_CALL(shipClassContainer, getById(1)).Times(1).WillRepeatedly(ReturnRef(shipClass));

    Server::Game::ObjectFactory factory(db, shipClassContainer);

    boost::shared_ptr<Common::Game::Object::ObjectBase> object = factory.create(node);

    auto & ship = dynamic_cast<Common::Game::Object::Ship&>(*object);

    ASSERT_EQ(typeid(Common::Game::Object::Ship), typeid(*object));
    ASSERT_EQ(1u, object->getId());
    ASSERT_EQ(2u, dynamic_cast<Common::Game::Object::OwnedObjectBase&>(*object).getOwnerId());

    Common::Game::Position position = object->getPosition();
    ASSERT_EQ(3, position.getX());
    ASSERT_EQ(4, position.getY());
    ASSERT_EQ(5, position.getZ());
}

TEST_F(ObjectFactoryTest, CreateShip)
{
    Server::DataBase::DataBase db;

    Server::Game::ShipClassMock shipClass;
    EXPECT_CALL(shipClass, applyTo(_)).Times(1);

    Server::Game::ShipClassContainerMock shipClassContainer;
    EXPECT_CALL(shipClassContainer, getById(1)).Times(1).WillRepeatedly(ReturnRef(shipClass));

    Server::Game::ObjectFactory factory(db, shipClassContainer);

    auto object = factory.createShip(1, 2);

    ASSERT_EQ(typeid(Common::Game::Object::Ship), typeid(*object));
    ASSERT_EQ(0u, object->getId()); // invalid id 
    ASSERT_EQ(2u, dynamic_cast<Common::Game::Object::OwnedObjectBase&>(*object).getOwnerId());
}

TEST_F(ObjectFactoryTest, CreateAsteroidFromDbNode)
{
    Server::DataBase::DataBaseNode node("object");
    node.setValue("type", "Asteroid");
    node.setValue("model", "Asteroid.model");
    node.setValue("id", 1);
    node.setValue("x", 3);
    node.setValue("y", 4);
    node.setValue("z", 5);
    node.setValue("integrity", 100);
    node.setValue("carbon", 10);
    node.setValue("helium", 20);

    Server::DataBase::DataBase db;
    Server::Game::ShipClassContainerMock shipClassContainer;
    Server::Game::ObjectFactory factory(db, shipClassContainer);

    boost::shared_ptr<Common::Game::Object::ObjectBase> object = factory.create(node);

    ASSERT_TRUE(object.get());

    auto & asteroid = dynamic_cast<Common::Game::Object::Asteroid&>(*object);

    Common::Game::Position position = asteroid.getPosition();
    EXPECT_EQ(3, position.getX());
    EXPECT_EQ(4, position.getY());
    EXPECT_EQ(5, position.getZ());

    EXPECT_EQ(100u, asteroid.getIntegrity());

    asteroid.visitCargoHold(
        [](Common::Game::Object::CargoHold & cargoHold) -> void
        {
            EXPECT_EQ(10u, cargoHold.getCarbon()); 
            EXPECT_EQ(20u, cargoHold.getHelium()); 
        }
    );
}

