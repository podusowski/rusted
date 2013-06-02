#include <gtest/gtest.h>

#include <fstream>
#include <cstdio>

#include "DataBase/DataBase.hpp"
#include "DataBase/XmlDataProvider.hpp"

using namespace testing;
using namespace Server::DataBase;

class XmlDataProviderTest : public Test
{
public:
    XmlDataProviderTest() : m_xmlFile("/var/tmp/XmlDataProviderTest.db1.xml")
    {
        std::ofstream db1(m_xmlFile);
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
        std::remove(m_xmlFile.c_str());
    }

    const std::string m_xmlFile;
};

TEST_F(XmlDataProviderTest, DataBaseLoad)
{
    DataBase db;
    XmlDataProvider xmlProvider(db, m_xmlFile);
    xmlProvider.load();

    EXPECT_EQ(1, db.getRoot()
                 .getFirstChild("entities")
                 .getFirstChild("entity")
                 .getValue<int>("id"));
}

