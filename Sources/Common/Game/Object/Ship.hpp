#pragma once

#include "Cake/DependencyInjection/Inject.hpp"

#include "Common/Game/Object/ObjectBase.hpp"
#include "Common/Game/Object/OwnedObjectBase.hpp"
#include "Common/Game/IRustedTime.hpp"
#include "IFlightTrajectory.hpp"

namespace Common
{
namespace Game
{
namespace Object
{

class Ship : public ObjectBase, public OwnedObjectBase
{
public:
    Ship();
    ~Ship();

    virtual Position getPosition();
    void setPosition(const Position &);
    void setCourse(Position course);
    void setSpeed(unsigned speed);
    unsigned getSpeed();
    void setIntegrity(unsigned); //override;

    virtual IFlightTrajectory::Description getTrajectoryDescription();
    void applyTrajectoryDescription(IFlightTrajectory::Description);

private:
    Cake::DependencyInjection::Inject<IRustedTime> m_time;
    Cake::DependencyInjection::Inject<IFlightTrajectory> m_trajectory;
};

}
}
}
