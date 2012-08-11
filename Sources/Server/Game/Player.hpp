#pragma once

#include "Common/Game/Object/ObjectBase.hpp"
#include "IPlayer.hpp"

namespace Server
{
namespace Game
{

class Player : public IPlayer
{
public:
    Player();
    void setId(int);
    int getId();

    void setState(PlayerState);
    PlayerState getState();

    void selectObject(Common::Game::Object::ObjectBase &);
    Common::Game::Object::ObjectBase & getSelectedObject();

    void focusObject(Common::Game::Object::ObjectBase &);
    Common::Game::Object::ObjectBase & getFocusedObject();

private:
    int m_id;
    PlayerState m_state;
    Common::Game::Object::ObjectBase * m_selectedObject;
    Common::Game::Object::ObjectBase * m_focusedObject;
};

}
}

