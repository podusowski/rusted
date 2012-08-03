#pragma once

#include <memory>

#include "DataBase/DataBase.hpp"
#include "Common/Configuration/Configuration.hpp"

namespace Server 
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

