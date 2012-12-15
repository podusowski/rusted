#pragma once

#include "Common/Game/Universe.hpp"
#include "Common/Game/Object/Ship.hpp"
#include "Network/IConnection.hpp"

namespace Server
{
namespace Services
{

class Utils
{
public:
    void sendObjectInfo(unsigned id, Common::Game::Universe &, Network::IConnection &);
    void sendObjectInfo(Common::Game::Object::ObjectBase &, Network::IConnection &);
    void sendShipCourseInfo(Common::Game::Object::Ship &, Network::IConnection &);

private:
    void sendShipInfo(Common::Game::Object::Ship &, Network::IConnection &);
};

}
}
