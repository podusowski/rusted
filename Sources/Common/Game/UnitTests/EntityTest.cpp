#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "DataBase/DataBase.hpp"
#include "Game/Entity.hpp"

#include "Common/Game/UnitTests/RustedTimeStub.hpp"

using namespace Common;

class EntityTest : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE (EntityTest);
    CPPUNIT_TEST(testEntityMovementBySimpleVector);
    CPPUNIT_TEST_SUITE_END ();

public:
    void testEntityMovementBySimpleVector();
};

CPPUNIT_TEST_SUITE_REGISTRATION (EntityTest);

void EntityTest::testEntityMovementBySimpleVector()
{
#if 0
    RustedTimeStub time;

    Common::Game::Entity entity(time, 1, 1, Common::Game::Entity::Position(0, 0, 0));

    CPPUNIT_ASSERT(1 == entity.getSpeed());

    Common::Point3<int> destination(100, 0, 0);
    entity.setCourse(destination);

    time.setSeconds(50);
    Common::Point3<int> p1 = entity.getPosition();
    CPPUNIT_ASSERT_EQUAL(50, p1.getX());
    CPPUNIT_ASSERT_EQUAL(0, p1.getY());
    CPPUNIT_ASSERT_EQUAL(0, p1.getZ());

    time.setSeconds(100);
    CPPUNIT_ASSERT(destination == entity.getPosition()); 

    destination.setY(50);
    entity.setCourse(destination);
    time.setSeconds(200);

    CPPUNIT_ASSERT_EQUAL(destination.getY(), entity.getPosition().getY());
#endif
}

