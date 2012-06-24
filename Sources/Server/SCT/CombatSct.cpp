#include <gtest/gtest.h>

#include "Core/Component.hpp"
#include "Core/Connection.hpp"
#include "Preconditions.hpp"
#include "UserFunctions.hpp"

TEST(CombatSct, Attack)
{
    std::string dbFile = "SampleDataBase.xml";

	SCT::Component component;
    component.setConfigValue("--database.provider", "xml");
    component.setConfigValue("--database.xml.filename", dbFile);
    component.start();

    boost::shared_ptr<SCT::Connection> connection1 = authorizeUser(component, "user1", "password"); 
    boost::shared_ptr<SCT::Connection> connection2 = authorizeUser(component, "user2", "password"); 

    Common::Messages::AttackObject attackObject;
    attackObject.attackerId = 1;
    attackObject.attackedId = 2;
    connection1->send(attackObject);

    connection1->receive<Common::Messages::AttackObject>();
    connection2->receive<Common::Messages::AttackObject>();

    auto shipInfo1 = connection1->receive<Common::Messages::ShipInfo>();
    auto shipInfo2 = connection2->receive<Common::Messages::ShipInfo>();

    ASSERT_EQ(90, shipInfo1->integrity);
    ASSERT_EQ(90, shipInfo2->integrity);
}
