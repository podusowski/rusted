#pragma once

#include <gtest/gtest.h>
#include <boost/shared_ptr.hpp>

#include "Cake/DependencyInjection/Registry.hpp"
#include "Common/Game/UnitTests/RustedTimeStub.hpp"
#include "Common/Game/Object/FlightTrajectory.hpp"

namespace Server
{

class AbstractTest : public testing::Test
{
public:
    AbstractTest()
    {
        using namespace Cake::DependencyInjection;

        m_time = boost::shared_ptr<Common::Game::IRustedTime>(new RustedTimeStub);
        Cake::DependencyInjection::forInterface<Common::Game::IRustedTime>().use(m_time);

        forInterface<Common::Game::Object::IFlightTrajectory>()
            .useFactory<GenericFactory0<Common::Game::Object::IFlightTrajectory, Common::Game::Object::FlightTrajectory> >();

        ON_CALL(getTimeMock(), getCurrentTime()).WillByDefault(testing::Return(Common::Game::TimeValue()));
    }

    virtual ~AbstractTest()
    {
        Cake::DependencyInjection::clear();
    }

    RustedTimeStub & getTimeMock()
    {
        return dynamic_cast<RustedTimeStub&>(*m_time);
    }

private:
    boost::shared_ptr<Common::Game::IRustedTime> m_time;
};

}
