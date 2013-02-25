#pragma once

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <boost/shared_ptr.hpp>

#include "Game/UnitTests/ObjectFactoryMock.hpp"
#include "Game/UnitTests/ShipClassContainerMock.hpp"
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

        m_objectFactory = boost::shared_ptr<Server::Game::IObjectFactory>(new Server::Game::ObjectFactoryMock);
        forInterface<Server::Game::IObjectFactory>().use(m_objectFactory);

        m_shipClassContainer = boost::shared_ptr<Game::IShipClassContainer>(new Game::ShipClassContainerMock());
        forInterface<Server::Game::IShipClassContainer>().use(m_shipClassContainer);

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

    Server::Game::ObjectFactoryMock & getObjectFactoryMock()
    {
        return dynamic_cast<Server::Game::ObjectFactoryMock&>(*m_objectFactory);
    }

    Server::Game::ShipClassContainerMock & getShipClassContainerMock()
    {
        return dynamic_cast<Server::Game::ShipClassContainerMock&>(*m_shipClassContainer);
    }

private:
    boost::shared_ptr<Common::Game::IRustedTime> m_time;
    boost::shared_ptr<Server::Game::IObjectFactory> m_objectFactory;
    boost::shared_ptr<Server::Game::IShipClassContainer> m_shipClassContainer;
};

}

