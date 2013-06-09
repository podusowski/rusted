#pragma once

#include <memory>

#include "Cake/Configuration/Configuration.hpp"
#include "Cake/DependencyInjection/Inject.hpp"

#include "DataBase/DataBaseNode.hpp"

namespace Server
{
namespace DataBase
{

class DataBase
{
public:
    DataBase();
    DataBaseNode & getRoot();
    void reset();
    void reload();

private:
    std::auto_ptr<DataBaseNode> m_root;
    Cake::DependencyInjection::Inject<Cake::Configuration::Configuration> m_cfg;
};

}
}

