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
    Asteroid();

    void setPosition(const Position &);
    Position getPosition();
    bool isMoving();

private:
    Position m_position;
};

}
}
}
