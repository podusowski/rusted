#include "Cake/Diagnostics/Logger.hpp"

#include "DataBase/DataProviderFactory.hpp"
#include "DataBase/DataBase.hpp"

using namespace Server::DataBase;

DataBase::DataBase()
{
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
    reset();

    for (int i = 1; i < 100; i++)
    {
        std::stringstream providerPrefix;
        providerPrefix << "database.provider" << i;

        if (m_cfg->hasValue(providerPrefix.str() + ".type"))
        {
            std::string provider = m_cfg->getValue<std::string>(providerPrefix.str() + ".type");
            std::string url = m_cfg->getValue<std::string>(providerPrefix.str() + ".url");

            LOG_INFO << "Reading data from provider #" << i << ":" << provider << "/" << url;

            DataProviderFactory providerFactory;
            auto dataProvider = providerFactory.create(provider, url);
            dataProvider->load(*m_root);
        }
    }
}

