#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <fstream>
#include <string>
#include <stdexcept>

#include "Configuration/Configuration.hpp"

class ConfigurationTest : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE (ConfigurationTest);
    CPPUNIT_TEST (testSimpleTypes);
    CPPUNIT_TEST (testNonExistingSetting);
    CPPUNIT_TEST (testCmdLineSettings);
    CPPUNIT_TEST_SUITE_END ();

public:
    void setUp();
    void testSimpleTypes();
    void testNonExistingSetting();
    void testCmdLineSettings();
};

CPPUNIT_TEST_SUITE_REGISTRATION (ConfigurationTest);

void ConfigurationTest::setUp()
{
    std::ofstream testConfigFile("/var/tmp/ConfigurationTest.testConfigFile.cfg");
    testConfigFile <<
            "someCategory1.someIntegerValueOf1=1\n"
            "someCategory2.someStringValueOfFoo=Foo\n"
            "someCategory2.someBoolValueOfTrue=1\n";
    testConfigFile.flush();
}

void ConfigurationTest::testSimpleTypes()
{
    const char * argv[] = {
        "appFilename",
        "--cfg",
        "/var/tmp/ConfigurationTest.testConfigFile.cfg"
    };
    Common::Configuration::Configuration configuration(3, argv); 

    CPPUNIT_ASSERT(1 == configuration.getValue<int>("someCategory1.someIntegerValueOf1"));
    CPPUNIT_ASSERT("Foo" == configuration.getValue<std::string>("someCategory2.someStringValueOfFoo"));
    CPPUNIT_ASSERT(true == configuration.getValue<bool>("someCategory2.someBoolValueOfTrue"));
}

void ConfigurationTest::testNonExistingSetting()
{
    const char * argv[] = {
        "appFilename",
        "--cfg",
        "/var/tmp/ConfigurationTest.testConfigFile.cfg"
    };
    Common::Configuration::Configuration configuration(3, argv); 

    CPPUNIT_ASSERT_THROW(configuration.getValue<int>("non-existing-setting"), std::out_of_range);
}

void ConfigurationTest::testCmdLineSettings()
{
    const char * argv[] = {
        "appFilename",
        "--cfg",
        "/var/tmp/ConfigurationTest.testConfigFile.cfg",
        "--someCategory1.someIntegerValueOf1",
        "2",
        "--someCmdLineValueOf1",
        "1"
    };
    Common::Configuration::Configuration configuration(7, argv); 

    CPPUNIT_ASSERT(2 == configuration.getValue<int>("someCategory1.someIntegerValueOf1"));
    CPPUNIT_ASSERT(1 == configuration.getValue<int>("someCmdLineValueOf1"));
}

