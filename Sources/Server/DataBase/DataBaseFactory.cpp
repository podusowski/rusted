#include <iostream>

#include "Cake/Diagnostics/Logger.hpp"

#include "DataBase/XmlDataProvider.hpp"
#include "DataBase/DataBaseFactory.hpp"

using namespace Server::DataBase;

DataBaseFactory::DataBaseFactory(Cake::Configuration::Configuration & cfg) : m_cfg(cfg)
{
}

boost::shared_ptr<DataBase> DataBaseFactory::create()
{
    boost::shared_ptr<DataBase> ret(new DataBase);
    return ret;
}

