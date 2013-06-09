#include <gtest/gtest.h>

#include <stdexcept>

#include "UnitTests/AbstractTest.hpp"
#include "DataBase/DataBaseNode.hpp"

using namespace Server::DataBase;

class DataBaseNodeTest : public Server::AbstractTest
{
};

TEST_F(DataBaseNodeTest, BasicTree)
{
    DataBaseNode root("root");
    DataBaseNode & entities = root.createChild("entities");

    EXPECT_EQ("root", root.getName());
    EXPECT_EQ(1u, root.getChildCount());

    DataBaseNode & entity1 = entities.createChild("entity");
    entity1.setValue("id", 1);

    EXPECT_EQ(1, root.getFirstChild("entities").getFirstChild("entity").getValue<int>("id"));
}

TEST_F(DataBaseNodeTest, NonExistingNode)
{
    DataBaseNode node("node");
    EXPECT_THROW(node.getFirstChild("nonExistingChild"), std::out_of_range);
}

TEST_F(DataBaseNodeTest, Iterators)
{
    DataBaseNode node("node");
    node.createChild("child").setValue("id", 1);
    node.createChild("child").setValue("id", 2);

    EXPECT_TRUE(node.getChilds().begin() != node.getChilds().end());

    auto childs = node.getChilds();
    auto it = childs.begin();

    EXPECT_EQ(1, (*it)->getValue<int>("id"));

    it++;

    EXPECT_EQ(2, (*it)->getValue<int>("id"));

    it++;

    EXPECT_TRUE(it == childs.end());
}

TEST_F(DataBaseNodeTest, IterateTroughValues)
{
    DataBaseNode node("node");

    node.setValue("val1", 1);

    auto values = node.getValues();
    ASSERT_EQ(1u, values.size());
    EXPECT_EQ("1", values["val1"]);
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

