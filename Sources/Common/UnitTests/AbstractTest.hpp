#pragma once

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <boost/shared_ptr.hpp>

#include "Cake/DependencyInjection/Registry.hpp"

#include "Common/Game/UnitTests/RustedTimeStub.hpp"
#include "Game/UnitTests/RustedTimeStub.hpp"
#include "Math/UnitTests/Spline3Mock.hpp"
#include "Game/Object/UnitTests/FlightTrajectoryMock.hpp"

namespace Common
{

class AbstractTest : public testing::Test
{
public:
    AbstractTest() :
        m_time(new RustedTimeStub),
        m_spline(new Common::Math::Spline3Mock),
        m_flightTrajectory(new Common::Game::Object::FlightTrajectoryMock)
    {
        using namespace Cake::DependencyInjection;

        Cake::DependencyInjection::clear();
        forInterface<Common::Game::IRustedTime>().use(m_time);
        forInterface<Common::Math::ISpline3>().use(m_spline);
        forInterface<Common::Game::Object::IFlightTrajectory>().use(m_flightTrajectory);
    }

    virtual ~AbstractTest()
    {
        Cake::DependencyInjection::clear();
    }

    RustedTimeStub & getTimeMock()
    {
        return *dynamic_cast<RustedTimeStub*>(m_time.get());
    }

    Common::Math::Spline3Mock & getSpline3Mock()
    {
        return *dynamic_cast<Common::Math::Spline3Mock*>(m_spline.get());
    }

private:
    boost::shared_ptr<Common::Game::IRustedTime> m_time;
    boost::shared_ptr<Common::Math::ISpline3> m_spline;
    boost::shared_ptr<Common::Game::Object::IFlightTrajectory> m_flightTrajectory;
};

}
