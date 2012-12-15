#pragma once

#include "ObjectBase.hpp"
#include "Common/Game/Position.hpp"

namespace Common
{
namespace Game
{
namespace Object
{

class Asteroid : public ObjectBase
{
public:
    void setPosition(const Position &);
    Position getPosition();

private:
    Position m_position;
};

}
}
}
