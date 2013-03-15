#include "Server/Services/RustedTimeService.hpp"

using namespace Server::Services;

RustedTimeService::RustedTimeService(Common::Game::IRustedTime & time) : m_time(time)
{
}

void RustedTimeService::handle(const Common::Messages::RustedTimeEpochReq &, Network::IConnection & connection)
{
    auto time = m_time.getCurrentTime();

    Common::Messages::RustedTimeEpochResp resp;
    resp.seconds = time.getSeconds();
    resp.miliSeconds = time.getMiliseconds();

    connection.send(resp);
}
