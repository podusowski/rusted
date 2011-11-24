#pragma once

namespace Common
{
namespace Game
{
namespace Object
{

class IOwnedObject
{
public:
    virtual unsigned getPlayerId() = 0;

    ~IOwnedObject() {}
};

}
}
}
