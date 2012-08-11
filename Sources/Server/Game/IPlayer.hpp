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

class IPlayer
{
public:
    virtual ~IPlayer() {}

    virtual void setId(int) = 0;
    virtual int getId() = 0;

    virtual void setState(PlayerState) = 0;
    virtual PlayerState getState() = 0;

    virtual void selectObject(Common::Game::Object::ObjectBase &) = 0;
    virtual Common::Game::Object::ObjectBase & getSelectedObject() = 0;

    virtual void focusObject(Common::Game::Object::ObjectBase &) = 0;
    virtual Common::Game::Object::ObjectBase & getFocusedObject() = 0;
};

}
}

