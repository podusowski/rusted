#pragma once

#include <memory>

#include "DataBase/DataBase.hpp"
#include "Cake/Configuration/Configuration.hpp"

namespace Server 
{
namespace DataBase
{

class DataBaseFactory
{
public:
    DataBaseFactory(Cake::Configuration::Configuration & cfg);
    DataBase & create();

private:
    std::auto_ptr<DataBase> m_db;
    Cake::Configuration::Configuration & m_cfg;
};

}
}

