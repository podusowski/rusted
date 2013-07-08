#include "SociSessionFactory.hpp"

using namespace Server::DataBase;

boost::shared_ptr<soci::session> SociSessionFactory::create()
{
    return boost::shared_ptr<soci::session>();
}
