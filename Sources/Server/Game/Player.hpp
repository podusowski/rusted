#pragma once

#include "Common/Game/Object/ObjectBase.hpp"

namespace Server
{
namespace Game
{

enum PlayerState
{
    PLAYER_STATE_NEW,
    PLAYER_STATE_AUTHORIZED
};

class Player
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

