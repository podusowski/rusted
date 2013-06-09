#include "Cake/Diagnostics/Logger.hpp"

#include "DataBase/DataProviderFactory.hpp"
#include "DataBase/DataBase.hpp"

using namespace Server::DataBase;

DataBase::DataBase()
{
    reset();
    reload();
}

DataBaseNode & DataBase::getRoot()
{
    return *m_root;
}

void DataBase::reset()
{
    LOG_INFO << "Resetting database";
    m_root.reset(new DataBaseNode("root"));
}

void DataBase::reload()
{
    if (m_cfg->hasValue("database.provider"))
    {
        std::string provider = m_cfg->getValue<std::string>("database.provider");
        std::string url = m_cfg->getValue<std::string>("database.xml.filename");

        LOG_INFO << "Reloading database using: " << provider << "/" << url;

        DataProviderFactory providerFactory;
        auto dataProvider = providerFactory.create(provider, url);
        dataProvider->load(*m_root);
    }
    else
    {
        LOG_WARN << "No database provider defined";
    }
}

