#pragma once

#include "Common/Game/Position.hpp"

namespace Common
{
namespace Game
{
namespace Object
{

class ObjectBase
{
public:
    unsigned getId() { return m_id; }
    void setId(unsigned id) { m_id = id; }

    virtual Position getPosition() = 0;
    virtual void setPosition(const Position &) = 0;

    virtual ~ObjectBase() {}

private:
    unsigned m_id;
};

}
}
}
