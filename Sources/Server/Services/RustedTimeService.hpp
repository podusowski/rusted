#pragma once

#include "Server/Services/AbstractService.hpp"
#include "Common/Game/IRustedTime.hpp"

namespace Server
{
namespace Services
{

class RustedTimeService : public AbstractService<RustedTimeService>
{
public:
    RustedTimeService(Common::Game::IRustedTime &);

    void handle(const Common::Messages::RustedTimeEpochReq &, Network::IConnection &);
    void handle(const Common::Messages::AbstractMessage &, Network::IConnection &) {}

private:
    Common::Game::IRustedTime & m_time;
};

}
}
