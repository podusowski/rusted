#pragma once

#include "Common/Game/Object/IObject.hpp"
#include "Common/Game/Object/IFlyableObject.hpp"
#include "Common/Game/Object/IOwnedObject.hpp"

namespace Common
{
namespace Game
{
namespace Object
{

class Ship : public IObject, public IFlyableObject, public IOwnedObject
{
public:
    Position getPosition();
    void setPosition(const Position &);
    void setCourse(Position course);
    unsigned getOwnerId();
};

}
}
}
