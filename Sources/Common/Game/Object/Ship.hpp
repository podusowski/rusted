#pragma once

#include "Cake/DependencyInjection/Inject.hpp"

#include "Common/Game/Object/IObject.hpp"
#include "Common/Game/Object/IFlyableObject.hpp"
#include "Common/Game/Object/IOwnedObject.hpp"
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
    unsigned startTime;
};

class Ship : public IObject, public IFlyableObject, public IOwnedObject
{
public:
    Position getPosition();
    void setPosition(const Position &);
    void setCourse(Position course);
    unsigned getOwnerId();

private:
    Cake::DependencyInjection::Inject<IRustedTime> m_time;

    Position m_position;
    Course m_course;
};

}
}
}
