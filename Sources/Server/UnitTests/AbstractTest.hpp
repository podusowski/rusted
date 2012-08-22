#pragma once

#include <gtest/gtest.h>
#include <boost/shared_ptr.hpp>

#include "Cake/DependencyInjection/Registry.hpp"
#include "Common/Game/UnitTests/RustedTimeStub.hpp"

namespace Server
{

class AbstractTest : public testing::Test
{
public:
    AbstractTest()
    {
        m_time = boost::shared_ptr<Common::Game::IRustedTime>(new RustedTimeStub);
        Cake::DependencyInjection::forInterface<Common::Game::IRustedTime>().use(m_time);

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
