#pragma once

namespace Common
{
namespace Game
{
namespace Object
{

class OwnedObjectBase
{
public:
    virtual void setOwnerId(unsigned id) { m_owner = id; }
    virtual unsigned getOwnerId() { return m_owner; }

    virtual ~OwnedObjectBase() {}

private:
    unsigned m_owner;
};

}
}
}
