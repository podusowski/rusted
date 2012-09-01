#pragma once

#include <boost/optional.hpp>

#include "Common/Game/Object/ObjectBase.hpp"

namespace Common
{
namespace Game
{

class Player
{
public:
    Player();
    ~Player();

    void setId(int id);
    int getId();

    void focusObject(Common::Game::Object::ObjectBase &);
    Common::Game::Object::ObjectBase & getFocusedObject();

private:
    boost::optional<int> m_id;
    Common::Game::Object::ObjectBase * m_focusedObject;
};

}
}

