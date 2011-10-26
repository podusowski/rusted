#include "Server/Services/RustedTimeService.hpp"

using namespace Server::Services;

RustedTimeService::RustedTimeService(Common::Game::IRustedTime & time) : m_time(time)
{
}

void RustedTimeService::handle(const Common::Messages::RustedTimeEpochReq &, Network::IConnection & connection)
{
    Common::Messages::RustedTimeEpochResp resp;
    resp.time = m_time.getSeconds();
    connection.send(resp);
}
