#pragma once

#include "ObjectBase.hpp"
#include "Common/Game/Position.hpp"

namespace Common
{
namespace Game
{
namespace Object
{

class Asteroid : public ObjectBase
{
public:
    Asteroid();

    void setPosition(const Position &);
    Position getPosition();

    unsigned getHelium();
    void setHelium(unsigned);

    unsigned getCarbon();
    void setCarbon(unsigned);

private:
    Position m_position;

    unsigned m_helium;
    unsigned m_carbon;
};

}
}
}
