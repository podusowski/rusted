#pragma once

namespace Common
{
namespace Game
{
namespace Object
{

class IMovableObject
{
public:
    void setCourse(Position course) = 0;

    virtual ~IMovableObject() {}
};

}
}
}
