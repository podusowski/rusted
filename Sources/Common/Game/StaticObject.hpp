#pragma once

#include "Common/Point3.hpp"

namespace Common
{
namespace Game
{

class StaticObject
{
public:
    typedef Common::Point3<int> Position;

    StaticObject(int id, Position position);
    int getId();
    Position getPosition();

private:
    int m_id;
    Position m_position;
};

}
}
