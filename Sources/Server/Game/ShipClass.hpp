#pragma once

#include "Common/Game/Object/Ship.hpp"
#include "DataBase/DataBaseNode.hpp"

namespace Server
{
namespace Game
{

class ShipClass
{
public:
    ShipClass(DataBase::DataBaseNode &);

    void applyTo(Common::Game::Object::Ship &);

private:
    unsigned m_speed;
    unsigned m_integrity;
};

}
}

