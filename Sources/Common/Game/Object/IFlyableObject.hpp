#pragma once

namespace Common
{
namespace Game
{
namespace Object
{

class IFlyableObject
{
public:
    virtual void setCourse(Position course) = 0;

    virtual ~IFlyableObject() {}
};

}
}
}
