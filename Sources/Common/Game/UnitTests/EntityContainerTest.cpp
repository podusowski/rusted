#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "DataBase/DataBase.hpp"
#include "Game/EntityContainer.hpp"
#include "Common/Game/UnitTests/RustedTimeStub.hpp"

using namespace Common;

class EntityContainerTest : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE (EntityContainerTest);
    CPPUNIT_TEST(testSimpleUnitAddAndReferencingById);
    CPPUNIT_TEST(testPlayerIterator);
    CPPUNIT_TEST(testLoadingFromDataBase);
    CPPUNIT_TEST_SUITE_END ();

public:
    void testSimpleUnitAddAndReferencingById();
    void testPlayerIterator();
    void testLoadingFromDataBase();
};

CPPUNIT_TEST_SUITE_REGISTRATION (EntityContainerTest);

const int PLAYER_ID = 1;

void EntityContainerTest::testSimpleUnitAddAndReferencingById()
{
    DataBase::DataBase db;
    RustedTimeStub time;
    Common::Game::EntityContainer container(db, time);
    Common::Game::Entity & entity = container.create(PLAYER_ID);

    CPPUNIT_ASSERT_NO_THROW(container.getById(entity.getId()));

    CPPUNIT_ASSERT(db.getRoot().getFirstChild("entities").getChildCount() == 1);

    // sadly, we only assume that we don't have that id ...
    CPPUNIT_ASSERT_THROW(container.getById(99999), std::out_of_range);
}

void EntityContainerTest::testPlayerIterator()
{
    DataBase::DataBase db;
    RustedTimeStub time;
    Common::Game::EntityContainer container(db, time);
    Common::Game::Entity & entity = container.create(PLAYER_ID);

    std::auto_ptr<Common::Game::EntityContainer::Container> playerEntities = container.getByPlayerId(PLAYER_ID);

    Common::Game::EntityContainer::Container::iterator it = playerEntities->begin();

    CPPUNIT_ASSERT(it != playerEntities->end());
    CPPUNIT_ASSERT_EQUAL(entity.getId(), (*it)->getId());

    it++;

    CPPUNIT_ASSERT(it == playerEntities->end());
}

void EntityContainerTest::testLoadingFromDataBase()
{
    DataBase::DataBase db;
    RustedTimeStub time;

    DataBase::DataBaseNode & entitiesNode = db.getRoot().createChild("entities");
    DataBase::DataBaseNode & entityNode = entitiesNode.createChild("1");
    entityNode.setValue("id", 1);
    entityNode.setValue("player_id", PLAYER_ID);
    entityNode.setValue("x", 1);
    entityNode.setValue("y", 2);
    entityNode.setValue("z", 3);

    Common::Game::EntityContainer container(db, time);

    std::auto_ptr<Common::Game::EntityContainer::Container> playerEntities = container.getByPlayerId(PLAYER_ID);

    Common::Game::EntityContainer::Container::iterator it = playerEntities->begin();
    CPPUNIT_ASSERT(it != playerEntities->end());
    CPPUNIT_ASSERT_EQUAL(1, (*it)->getId());
}

