#pragma once

#include "Common/Game/Object/Ship.hpp"
#include "DataBase/DataBaseNode.hpp"
#include "IShipClass.hpp"

namespace Server
{
namespace Game
{

class ShipClass : public IShipClass
{
public:
    ShipClass(DataBase::DataBaseNode &);

    void applyTo(Common::Game::Object::Ship &);
    std::vector<AvailableAction> getAvailableActions();
    unsigned getId();
    std::string getName();

private:
    unsigned m_id;
    std::string m_name;
    unsigned m_speed;
    unsigned m_integrity;
    std::string m_mesh;
    unsigned m_capacity;
    std::vector<AvailableAction> m_availableActions;
};

}
}

