#pragma once

namespace Common
{
namespace Game
{
namespace Object
{

// TODO: should object have responsibility to track ownership?
class IOwnedObject
{
public:
    virtual unsigned getOwnerId() = 0;

    ~IOwnedObject() {}
};

}
}
}
