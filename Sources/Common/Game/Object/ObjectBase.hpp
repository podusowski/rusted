#pragma once

#include <typeinfo>

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

inline std::ostream & operator<<(std::ostream & os, Common::Game::Object::ObjectBase & object)
{
    os << typeid(object).name() << ", position: " << object.getPosition();
    return os;
}

