#pragma once

#include "Server/Game/Actions/IAction.hpp"
#include "Server/Game/IPlayerContainer.hpp"
#include "Common/Game/Object/Ship.hpp"

namespace Server
{
namespace Game
{
namespace Actions
{

class Attack : public IAction
{
public:
    Attack(IPlayerContainer &, Common::Game::Object::Ship & focusedShip, Common::Game::Object::ObjectBase & selectedObject);
    void execute();

private:
    void sendShipInfo(Common::Game::Object::Ship &, Network::IConnection &);

    IPlayerContainer & m_playerContainer;
    Common::Game::Object::Ship & m_focusedShip;
    Common::Game::Object::ObjectBase & m_selectedObject;
};

}
}
}

