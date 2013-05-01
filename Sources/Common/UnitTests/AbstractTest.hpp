#pragma once

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <boost/shared_ptr.hpp>

#include "Cake/DependencyInjection/Registry.hpp"

#include "Common/Game/UnitTests/RustedTimeStub.hpp"
#include "Game/UnitTests/RustedTimeStub.hpp"
#include "Math/UnitTests/Spline3Mock.hpp"

namespace Common
{

class AbstractTest : public testing::Test
{
public:
    AbstractTest() :
        m_time(new RustedTimeStub),
        m_spline(new Common::Math::Spline3Mock)
    {
        using namespace Cake::DependencyInjection;

        forInterface<Common::Game::IRustedTime>().use(m_time);
        forInterface<Common::Math::ISpline3>().use(m_spline);
    }

    virtual ~AbstractTest()
    {
        Cake::DependencyInjection::clear();
    }

    RustedTimeStub & getTimeMock()
    {
        return *dynamic_cast<RustedTimeStub*>(m_time.get());
    }

private:
    boost::shared_ptr<Common::Game::IRustedTime> m_time;
    boost::shared_ptr<Common::Math::ISpline3> m_spline;
};

}
