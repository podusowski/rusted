#pragma once

#include <boost/optional.hpp>

#include "IPlayer.hpp"
#include "Common/Game/Object/ObjectBase.hpp"

namespace Common
{
namespace Game
{

class Player : public IPlayer
{
public:
    Player();
    ~Player();

    void setId(int id);
    int getId();

    void setState(PlayerState);
    PlayerState getState();

    void selectObject(Common::Game::Object::ObjectBase &);
    Common::Game::Object::ObjectBase & getSelectedObject();

    void focusObject(Common::Game::Object::ObjectBase &);
    Common::Game::Object::ObjectBase & getFocusedObject();

private:
    boost::optional<int> m_id;
    PlayerState m_state;
    Common::Game::Object::ObjectBase * m_selectedObject;
    Common::Game::Object::ObjectBase * m_focusedObject;
};

}
}

