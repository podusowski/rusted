#pragma once

#include "DataBase/DataBase.hpp"
#include "ShipClass.hpp"

namespace Server
{
namespace Game
{

class ShipClassContainer
{
public:
    ShipClassContainer(DataBase::DataBase &);
    ~ShipClassContainer();

    ShipClass & getById(unsigned);

private:
    void loadFromDataBase();

    DataBase::DataBase & m_db;
    std::map<unsigned, boost::shared_ptr<ShipClass> > m_shipClassMap;
};

}
}
