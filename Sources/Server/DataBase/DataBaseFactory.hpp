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
    boost::shared_ptr<DataBase> create();

private:
    Cake::Configuration::Configuration & m_cfg;
};

}
}

