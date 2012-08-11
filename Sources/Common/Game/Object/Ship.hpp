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
    Position start;
    Position destination;
    TimeValue startTime;
};

class Ship : public ObjectBase, public IFlyableObject, public OwnedObjectBase
{
public:
    Ship();
    virtual Position getPosition();
    void setPosition(const Position &);
    void setCourse(Position course);
    void setCourse(Course course);
    void setSpeed(unsigned speed);
    void setIntegrity(unsigned); //override;

    virtual Course getCourse();

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
