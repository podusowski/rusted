#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <stdexcept>

#include "DataBase/DataBaseNode.hpp"

using namespace Common::DataBase;

class DataBaseNodeTest : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE (DataBaseNodeTest);
    CPPUNIT_TEST (testBasicTree);
    CPPUNIT_TEST (testNonExistingNode);
    CPPUNIT_TEST (testIterators);
    //CPPUNIT_TEST (testNodeLink);
    CPPUNIT_TEST_SUITE_END ();

public:
    void testBasicTree();
    void testNonExistingNode();
    void testIterators();
    //void testNodeLink();

};

CPPUNIT_TEST_SUITE_REGISTRATION (DataBaseNodeTest);

void DataBaseNodeTest::testBasicTree()
{
    DataBaseNode root("root");
    DataBaseNode & entities = root.createChild("entities");

    CPPUNIT_ASSERT(1 == root.getChildCount());

    DataBaseNode & entity1 = entities.createChild("entity");
    entity1.setValue("id", 1);

    CPPUNIT_ASSERT_EQUAL(1, root.getFirstChild("entities").getFirstChild("entity").getValue<int>("id"));
}

void DataBaseNodeTest::testNonExistingNode()
{
    DataBaseNode node("node");
    CPPUNIT_ASSERT_THROW(node.getFirstChild("nonExistingChild"), std::out_of_range);
}

void DataBaseNodeTest::testIterators()
{
    DataBaseNode node("node");
    node.createChild("child").setValue("id", 1);
    node.createChild("child").setValue("id", 2);

    CPPUNIT_ASSERT(node.getChilds().begin() != node.getChilds().end());

    DataBaseNode::iterator it = node.getChilds().begin();

    CPPUNIT_ASSERT_EQUAL(1, (*it)->getValue<int>("id"));

    it++;

    CPPUNIT_ASSERT_EQUAL(2, (*it)->getValue<int>("id"));

    it++;

    CPPUNIT_ASSERT(it == node.getChilds().end());
}

/*
void DataBaseNodeTest::testNodeLink()
{
    DataBaseNode node("node");
    DataBaseNode & child1 = node.createChild("child1");
    DataBaseNode & child2 = node.createChild("child2");

    child1.setValue("foo", 1);

    DataBaseNode & linkToChild1 = child2.createLink(child1);

    CPPUNIT_ASSERT(linkToChild1.getValue<int>("foo"), child1.getValue<int>("foo"));

    linkToChild1.setValue("foo", 2);

    CPPUNIT_ASSERT(2 == child1.getValue<int>("foo"));
}
*/

