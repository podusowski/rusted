#pragma once

#include <soci.h>

#include "Cake/DependencyInjection/Inject.hpp"

#include "IShipClassContainer.hpp"
#include "DataBase/DataBase.hpp"
#include "ShipClass.hpp"

namespace Server
{
namespace Game
{

class ShipClassContainer : public IShipClassContainer
{
public:
    ShipClassContainer(DataBase::DataBase &);
    ~ShipClassContainer();

    IShipClass & getById(unsigned);

private:
    void loadFromDataBase();

    DataBase::DataBase & m_db;
    std::map<unsigned, boost::shared_ptr<ShipClass> > m_shipClassMap;

    Cake::DependencyInjection::Inject<soci::session> m_sociSession;
};

}
}
