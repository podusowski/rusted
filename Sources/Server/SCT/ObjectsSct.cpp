#include <gtest/gtest.h>

#include "Core/Component.hpp"
#include "Core/Connection.hpp"
#include "Preconditions.hpp"
#include "UserFunctions.hpp"

using namespace Common::Messages;

/* This flows represents the way client is aware of various object present in the game */

TEST(ObjectsSct, FetchVisibleObjects)
{
    auto component = SCT::make_default_component();
    component.start();

    std::shared_ptr<SCT::Connection> connection1 = authorizeUser(component, "user1", "password"); 

    Common::Messages::GetVisibleObjects getVisibleObjects;
    connection1->send(getVisibleObjects);

    // VisibleObjects contains only ids of the objects that are visible by the client, to get
    // more information, GetObjectInfo should be used for each of them

    std::shared_ptr<Common::Messages::VisibleObjects> visibleObjects = connection1->receive<Common::Messages::VisibleObjects>();
    EXPECT_EQ(5u, visibleObjects->objects.size());
}

TEST(ObjectsSct, GetObjectInfo_Ship)
{
    auto component = SCT::make_default_component();
    component.start();

    std::shared_ptr<SCT::Connection> connection1 = authorizeUser(component, "user1", "password"); 

    Common::Messages::GetObjectInfo getObjectInfo;
    getObjectInfo.id = 2;
    connection1->send(getObjectInfo);

    auto shipInfo = connection1->receive<Common::Messages::ShipInfo>();
    EXPECT_EQ(200, shipInfo->integrity);
    EXPECT_EQ(20, shipInfo->speed);
    EXPECT_EQ("Ship2.model", shipInfo->model);

    auto shipCourseInfo = connection1->receive<Common::Messages::ShipCourseInfo>();

    auto objectCargoInfo = connection1->receive<Common::Messages::ObjectCargoInfo>();
    EXPECT_EQ(2, objectCargoInfo->id);
    EXPECT_EQ(1000, objectCargoInfo->capacity);
}

TEST(ObjectsSct, GetObjectInfo_Asteroid)
{
    auto component = SCT::make_default_component();
    component.start();

    std::shared_ptr<SCT::Connection> connection1 = authorizeUser(component, "user1", "password"); 

    Common::Messages::GetObjectInfo getObjectInfo;
    getObjectInfo.id = 3;
    connection1->send(getObjectInfo);

    auto asteroidInfo = connection1->receive<Common::Messages::AsteroidInfo>();
    EXPECT_EQ(getObjectInfo.id, asteroidInfo->objectId);
    EXPECT_EQ("Asteroid.model", asteroidInfo->model);
    EXPECT_EQ(100, asteroidInfo->x);
    EXPECT_EQ(100, asteroidInfo->y);
    EXPECT_EQ(100, asteroidInfo->z);

    auto objectCargoInfo = connection1->receive<Common::Messages::ObjectCargoInfo>();
    EXPECT_EQ(getObjectInfo.id, objectCargoInfo->id);
    EXPECT_EQ(100, objectCargoInfo->carbon);
    EXPECT_EQ(100, objectCargoInfo->helium);
}

TEST(ObjectsSct, DataBaseUpdatedWithNewObjects)
{
    auto component = SCT::make_default_component();
    component.start();

    std::shared_ptr<SCT::Connection> connection1 = authorizeUser(component, "user1", "password");

    auto sociSession = component.createSociSession();

    sociSession->once <<
        "INSERT INTO objects (id, type, class, x, y, z, owner, integrity, carbon, helium)"
        "VALUES(10, \"Ship\", 1, 100, 100, 100, 1, 100, 0, 0)";

    auto administrationConnection = component.createAdministrationConnection();

    Common::Messages::ReloadDatabase reloadDatabase;
    administrationConnection->send(reloadDatabase);

//    connection1->receive<Common::Messages::PlayerShips>();
    connection1->receive<Common::Messages::VisibleObjects>();
}

