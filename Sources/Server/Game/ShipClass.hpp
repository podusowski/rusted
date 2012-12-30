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

private:
    unsigned m_id;
    unsigned m_speed;
    unsigned m_integrity;
    std::string m_mesh;
    std::vector<AvailableAction> m_availableActions;
};

}
}

