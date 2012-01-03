#pragma once

#include "ObjectBase.hpp"
#include "Common/Game/Position.hpp"

namespace Common
{
namespace Game
{
namespace Object
{

class StaticObject : public ObjectBase
{
    void setPosition(const Position &);
    Position getPosition();

private:
    Position m_position;
};

}
}
}
