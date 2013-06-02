#include <gtest/gtest.h>

#include <fstream>

#include "DataBase/DataBase.hpp"
#include "DataBase/XmlDataProvider.hpp"

using namespace testing;
using namespace Server::DataBase;

class XmlDataProviderTest : public Test
{
public:
    void SetUp()
    {
        std::ofstream db1("/var/tmp/XmlDataProviderTest.db1.xml");
        db1 <<
            "<root>\n"
            "<entities>\n"
            "<entity id=\"1\" />\n"
            "</entities>\n"
            "</root>\n";
        db1.flush();
    }
};

TEST(XmlDataProviderTest, DataBaseLoad)
{
    DataBase db;
    XmlDataProvider xmlProvider(db, "/var/tmp/XmlDataProviderTest.db1.xml");
    xmlProvider.load();

    EXPECT_EQ(1, db.getRoot()
                 .getFirstChild("entities")
                 .getFirstChild("entity")
                 .getValue<int>("id"));
}

