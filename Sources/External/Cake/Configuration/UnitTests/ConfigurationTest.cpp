#include <gtest/gtest.h>

#include <fstream>
#include <string>
#include <stdexcept>

#include "Configuration/Configuration.hpp"

using namespace testing;

class ConfigurationTest : public Test
{
protected:
    virtual void SetUp()
    {
        std::ofstream testConfigFile("/var/tmp/ConfigurationTest.testConfigFile.cfg");
        testConfigFile <<
                "someCategory1.someIntegerValueOf1=1\n"
                "someCategory2.someStringValueOfFoo=Foo\n"
                "someCategory2.someBoolValueOfTrue=1\n";
        testConfigFile.flush();
    }
};

TEST_F(ConfigurationTest, SimpleTypes)
{
    const char * argv[] = {
        "appFilename",
        "--cfg",
        "/var/tmp/ConfigurationTest.testConfigFile.cfg"
    };
    Common::Configuration::Configuration configuration(3, argv); 

    EXPECT_TRUE(1 == configuration.getValue<int>("someCategory1.someIntegerValueOf1"));
    EXPECT_TRUE("Foo" == configuration.getValue<std::string>("someCategory2.someStringValueOfFoo"));
    EXPECT_TRUE(true == configuration.getValue<bool>("someCategory2.someBoolValueOfTrue"));
}

TEST_F(ConfigurationTest, NonExistingSetting)
{
    const char * argv[] = {
        "appFilename",
        "--cfg",
        "/var/tmp/ConfigurationTest.testConfigFile.cfg"
    };
    Common::Configuration::Configuration configuration(3, argv); 

    EXPECT_ANY_THROW(configuration.getValue<int>("non-existing-setting"));
}

TEST_F(ConfigurationTest, CmdLineSettings)
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

    EXPECT_TRUE(2 == configuration.getValue<int>("someCategory1.someIntegerValueOf1"));
    EXPECT_TRUE(1 == configuration.getValue<int>("someCmdLineValueOf1"));
}

TEST_F(ConfigurationTest, AppName)
{
    const char * argv[] = {
        "/some/directory/appFilename"
    };
    Common::Configuration::Configuration configuration(1, argv);

    EXPECT_TRUE("/some/directory/appFilename" == configuration.getAppName());
}

