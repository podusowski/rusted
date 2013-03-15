#pragma once

#include "Cake/DependencyInjection/Inject.hpp"

#include "Common/Game/IRustedTime.hpp"
#include "Client/Services/AbstractService.hpp"
#include "Client/Network/IConnection.hpp"

namespace Client
{
namespace Services
{

class RustedTimeService : public AbstractService<RustedTimeService>
{
public:
    RustedTimeService(Network::IConnection &);

    void synchronize();

    void handle(const Common::Messages::RustedTimeEpochResp &);
    void handle(const Common::Messages::AbstractMessage &) {}

private:
    Network::IConnection & m_connection;
    Cake::DependencyInjection::Inject<Common::Game::IRustedTime> m_time;
    Common::Game::TimeValue m_syncRequestTime;
};

}
}
