#pragma once

#include "Common/Game/IRustedTime.hpp"
#include "Client/Services/AbstractService.hpp"
#include "Client/Network/Connection.hpp"

namespace Client
{
namespace Services
{

class RustedTimeService : public AbstractService<RustedTimeService>
{
public:
    RustedTimeService(Network::Connection &, 
                      Common::Game::IRustedTime &);

    void synchronize();

    void handle(const Common::Messages::RustedTimeEpochResp &);
    void handle(const Common::Messages::AbstractMessage &) {}

private:
    Network::Connection & m_connection;
    Common::Game::IRustedTime & m_time;    
};

}
}
