#pragma once

#include <typeinfo>

#include "Cake/DependencyInjection/Inject.hpp"
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
    ObjectBase() : m_id(0), m_integrity(100) {}

    unsigned getId() { return m_id; }
    void setId(unsigned id) { m_id = id; }

    virtual Position getPosition() = 0;
    virtual void setPosition(const Position &) = 0;

    virtual unsigned getIntegrity() { return m_integrity; }
    virtual void setIntegrity(unsigned integrity) { m_integrity = integrity; }

    virtual ~ObjectBase() {}

private:
    unsigned m_id;
    unsigned m_integrity;
};

}
}
}

inline std::ostream & operator<<(std::ostream & os, Common::Game::Object::ObjectBase & object)
{
    os << CAKE_DEPENDENCY_INJECTION_TYPENAME(object) << " " << object.getPosition();
    return os;
}

