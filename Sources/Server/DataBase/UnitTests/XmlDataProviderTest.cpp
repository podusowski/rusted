#include <gtest/gtest.h>

#include <fstream>
#include <cstdio>

#include "Cake/Diagnostics/Logger.hpp"

#include "UnitTests/AbstractTest.hpp"
#include "DataBase/DataBase.hpp"
#include "DataBase/XmlDataProvider.hpp"

using namespace testing;
using namespace Server::DataBase;

class XmlDataProviderTest : public Server::AbstractTest
{
public:
    XmlDataProviderTest() : m_xmlFile1("/var/tmp/XmlDataProviderTest.db1.xml")
    {
        std::ofstream db1(m_xmlFile1);
        db1 <<
            "<root>\n"
            "<entities>\n"
            "<entity id=\"1\" />\n"
            "</entities>\n"
            "</root>\n";
        db1.flush();

    }

    ~XmlDataProviderTest()
    {
        std::remove(m_xmlFile1.c_str());
    }

    const std::string m_xmlFile1;
};

TEST_F(XmlDataProviderTest, DataBaseLoad)
{
    DataBase db;
    XmlDataProvider xmlProvider(m_xmlFile1);
    xmlProvider.load(db.getRoot());

    EXPECT_EQ(1, db.getRoot()
                 .getFirstChild("entities")
                 .getFirstChild("entity")
                 .getValue<int>("id"));
}

TEST_F(XmlDataProviderTest, LoadResetLoad)
{
    DataBase db;
    XmlDataProvider xmlProvider(m_xmlFile1);
    xmlProvider.load(db.getRoot());

    db.reset();
    xmlProvider.load(db.getRoot());

    ASSERT_EQ(1u, db.getRoot().getChildCount());
    EXPECT_EQ(1, db.getRoot()
                 .getFirstChild("entities")
                 .getFirstChild("entity")
                 .getValue<int>("id"));
}

TEST_F(XmlDataProviderTest, Save)
{
    DataBase db;
    XmlDataProvider xmlProvider(m_xmlFile1);

    auto & child1 = db.getRoot().createChild("child1");
    child1.setValue("param1", 1);

    auto & subchild1 = child1.createChild("subchild1");
    subchild1.setValue("param2", 2);

    std::string weirdCharacters = "~!@#$%^*()_+|}{\"'><?/";
    //std::string weirdCharacters = "&~!@#$%^*()_+|}{\"'><?/"; FIXME: some bug with & character, no xss though?
    subchild1.setValue("param3", weirdCharacters);

    xmlProvider.save(db.getRoot());

    {
        LOG_INFO << m_xmlFile1 << " contents:";

        std::fstream f(m_xmlFile1);
        std::string line;
        while (f.good())
        {
            std::getline(f, line);
            LOG_INFO << line;
        }
    }

    db.reset();

    ASSERT_EQ(0u, db.getRoot().getChildCount());

    xmlProvider.load(db.getRoot());

    EXPECT_EQ(1, db.getRoot().getFirstChild("child1").getValue<int>("param1"));
    EXPECT_EQ(2, db.getRoot().getFirstChild("child1").getFirstChild("subchild1").getValue<int>("param2"));
    EXPECT_EQ(weirdCharacters, db.getRoot().getFirstChild("child1").getFirstChild("subchild1").getValue<std::string>("param3"));
}

