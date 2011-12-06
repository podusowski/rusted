#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "DataBase/DataBase.hpp"
#include "DataBase/XmlDataProvider.hpp"

using namespace Common::DataBase;

class XmlDataProviderTest : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE (XmlDataProviderTest);
    CPPUNIT_TEST (testDataBaseLoad);
    CPPUNIT_TEST_SUITE_END ();

public:
    void setUp();
    void testDataBaseLoad();

};

CPPUNIT_TEST_SUITE_REGISTRATION (XmlDataProviderTest);

void XmlDataProviderTest::setUp()
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

void XmlDataProviderTest::testDataBaseLoad()
{
    DataBase db;
    XmlDataProvider xmlProvider(db, "/var/tmp/XmlDataProviderTest.db1.xml");

    CPPUNIT_ASSERT_EQUAL(1, db.getRoot()
                              .getFirstChild("entities")
                              .getFirstChild("entity")
                              .getValue<int>("id"));
}

