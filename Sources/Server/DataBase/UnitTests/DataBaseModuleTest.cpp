#include <gtest/gtest.h>

#include <fstream>
#include <boost/shared_ptr.hpp>

#include "Cake/Configuration/Configuration.hpp"
#include "DataBase/DataBase.hpp"
#include "DataBase/DataBaseFactory.hpp"

TEST(DataBaseModuleTest, XmlDataBaseRead)
{
    std::ofstream db1("/var/tmp/database.xml");
    db1 <<
            "<root>\n"
            "<entities>\n"
            "<entity id=\"1\" />\n"
            "</entities>\n"
            "</root>\n";
    db1.flush();

    Cake::Configuration::Configuration cfg;
    Server::DataBase::DataBaseFactory factory(cfg); 

    cfg.setValue("database.provider", "xml");
    cfg.setValue("database.xml.filename", "/var/tmp/database.xml");

    boost::shared_ptr<Server::DataBase::DataBase> db = factory.create();

    EXPECT_EQ(1, db->getRoot()
                 .getFirstChild("entities")
                 .getFirstChild("entity")
                 .getValue<int>("id"));
}

