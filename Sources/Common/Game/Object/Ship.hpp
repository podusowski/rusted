#pragma once

#include "Cake/DependencyInjection/Inject.hpp"

#include "Common/Game/Object/ObjectBase.hpp"
#include "Common/Game/Object/IFlyableObject.hpp"
#include "Common/Game/Object/OwnedObjectBase.hpp"
#include "Common/Game/IRustedTime.hpp"

namespace Common
{
namespace Game
{
namespace Object
{

struct Course
{
    Position course;
    TimeValue startTime;
};

class Ship : public ObjectBase, public IFlyableObject, public OwnedObjectBase
{
public:
    Ship();
    Position getPosition();
    void setPosition(const Position &);
    void setCourse(Position course);
    void setSpeed(unsigned speed);

private:
    Position calculatePosition(TimeValue time);

    Cake::DependencyInjection::Inject<IRustedTime> m_time;

    Position m_position;
    Course m_course;
    unsigned m_speed;
};

}
}
}
