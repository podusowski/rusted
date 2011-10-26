#include <iostream>

#include "DataBase/XmlDataProvider.hpp"
#include "DataBase/DataBaseFactory.hpp"
#include "Common/Logger/Logger.hpp"

using namespace Common::DataBase;

DataBaseFactory::DataBaseFactory(Common::Configuration::Configuration & cfg) : m_cfg(cfg)
{
}

DataBase & DataBaseFactory::create()
{
    m_db.reset(new DataBase());

    try
    {        
        if (m_cfg.getValue<std::string>("database.provider") == "xml")
        {
            LOG_INFO << "XmlDataProvider will be used as DB storage\n";
            XmlDataProvider provider(*m_db, m_cfg.getValue<std::string>("database.xml.filename"));
        }
        else
        {
            throw std::out_of_range("");
        }
    }
    catch (...)
    {
        LOG_WARN << "DataBase provider is not configured, clean DB will be created which won't do much good\n";
    }

    return *m_db;
}
