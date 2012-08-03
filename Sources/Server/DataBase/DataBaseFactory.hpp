#ifndef DATABASE_FACTORY_HPP
#define DATABASE_FACTORY_HPP

#include <memory>

#include "DataBase/DataBase.hpp"
#include "Common/Configuration/Configuration.hpp"

namespace Common 
{
namespace DataBase
{

class DataBaseFactory
{
public:
    DataBaseFactory(Common::Configuration::Configuration & cfg);
    DataBase & create();

private:
    std::auto_ptr<DataBase> m_db;
    Common::Configuration::Configuration & m_cfg;
};

}
}

#endif

