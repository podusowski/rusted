#pragma once

#include "Common/Game/Position.hpp"

namespace Common
{
namespace Game
{
namespace Object
{

class IObject
{
public:
    virtual Position getPosition() = 0;
    virtual void setPosition(const Position &) = 0;

    virtual ~IObject() {}
};

}
}
}
