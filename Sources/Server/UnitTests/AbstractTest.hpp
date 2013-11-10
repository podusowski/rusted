#pragma once

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "Game/UnitTests/UniverseDataBaseFacadeMock.hpp"
#include "Game/UnitTests/ShipClassContainerMock.hpp"
#include "Cake/DependencyInjection/Registry.hpp"
#include "Common/Game/UnitTests/RustedTimeStub.hpp"
#include "Common/Game/Object/FlightTrajectory.hpp"
#include "Math/UnitTests/Spline3Mock.hpp"

namespace Server
{

class AbstractTest : public testing::Test
{
public:
    AbstractTest() :
        m_spline(new Common::Math::Spline3Mock),
        m_cfg(new Cake::Configuration::Configuration)
    {
        using namespace Cake::DependencyInjection;

        m_time = std::shared_ptr<Common::Game::IRustedTime>(new RustedTimeStub);
        Cake::DependencyInjection::forInterface<Common::Game::IRustedTime>().use(m_time);

        forInterface<Common::Game::Object::IFlightTrajectory>()
            .useFactory<GenericFactory0<Common::Game::Object::IFlightTrajectory, Common::Game::Object::FlightTrajectory> >();

        m_universeDataBaseFacade = std::shared_ptr<Server::Game::IUniverseDataBaseFacade>(new Server::Game::UniverseDataBaseFacadeMock);
        forInterface<Server::Game::IUniverseDataBaseFacade>().use(m_universeDataBaseFacade);

        m_shipClassContainer = std::shared_ptr<Game::IShipClassContainer>(new Game::ShipClassContainerMock());
        forInterface<Server::Game::IShipClassContainer>().use(m_shipClassContainer);

        forInterface<Common::Math::ISpline3>().use(m_spline);
        forInterface<Cake::Configuration::Configuration>().use(m_cfg);

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

    Server::Game::UniverseDataBaseFacadeMock & getUniverseDataBaseFacadeMock()
    {
        return dynamic_cast<Server::Game::UniverseDataBaseFacadeMock&>(*m_universeDataBaseFacade);
    }

    Server::Game::ShipClassContainerMock & getShipClassContainerMock()
    {
        return dynamic_cast<Server::Game::ShipClassContainerMock&>(*m_shipClassContainer);
    }

    Common::Math::Spline3Mock & getSpline3Mock()
    {
        return *dynamic_cast<Common::Math::Spline3Mock*>(m_spline.get());
    }

private:
    std::shared_ptr<Common::Game::IRustedTime> m_time;
    std::shared_ptr<Server::Game::IUniverseDataBaseFacade> m_universeDataBaseFacade;
    std::shared_ptr<Server::Game::IShipClassContainer> m_shipClassContainer;
    std::shared_ptr<Common::Math::ISpline3> m_spline;
    std::shared_ptr<Cake::Configuration::Configuration> m_cfg;
};

}

