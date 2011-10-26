#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "Common/Configuration/Configuration.hpp"
#include "DataBase/DataBase.hpp"
#include "DataBase/DataBaseFactory.hpp"

using namespace Common;

class DataBaseModuleTest : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE (DataBaseModuleTest);
    CPPUNIT_TEST(testXmlDataBaseRead);
    CPPUNIT_TEST_SUITE_END ();

public:
    void testXmlDataBaseRead();
};

CPPUNIT_TEST_SUITE_REGISTRATION (DataBaseModuleTest);

void DataBaseModuleTest::testXmlDataBaseRead()
{
    std::ofstream db1("/var/tmp/database.xml");
    db1 <<
            "<root>\n"
            "<entities>\n"
            "<entity id=\"1\" />\n"
            "</entities>\n"
            "</root>\n";
    db1.flush();

    Common::Configuration::Configuration cfg;
    Common::DataBase::DataBaseFactory factory(cfg); 

    cfg.setValue("database.provider", "xml");
    cfg.setValue("database.xml.filename", "/var/tmp/database.xml");

    Common::DataBase::DataBase & db = factory.create();

    CPPUNIT_ASSERT_EQUAL(1, db.getRoot()
                              .getFirstChild("entities")
                              .getFirstChild("entity")
                              .getValue<int>("id"));
}

