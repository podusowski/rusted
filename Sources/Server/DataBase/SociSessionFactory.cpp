#include "Cake/Diagnostics/Logger.hpp"

#include "SociSessionFactory.hpp"

using namespace Server::DataBase;

boost::shared_ptr<soci::session> SociSessionFactory::create()
{
    std::string sociUrl = m_cfg->getValue<std::string>("database.url");
    LOG_INFO << "Creating database session on " << sociUrl;

    return boost::shared_ptr<soci::session>(new soci::session(sociUrl));
}

