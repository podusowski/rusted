#pragma once

#include "Common/Game/Object/ObjectBase.hpp"

namespace Client
{
namespace Views
{

class Object
{
public:
    Object(Common::Game::Object::ObjectBase &);

private:
    Common::Game::Object::ObjectBase & m_object;
};

}
}
