#pragma once

#include "Common/Game/Object/ObjectBase.hpp"

namespace Server
{
namespace Game
{

class ShipClassContainer
{
public:
    ShipClassContainer(DataBase::DataBase &);
    ~ShipClassContainer();

private:
    DataBase::DataBase & m_db;
};

}
}
