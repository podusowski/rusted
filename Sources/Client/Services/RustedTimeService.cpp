#include "Common/Logger/Logger.hpp"
#include "Client/Services/RustedTimeService.hpp"

using namespace Client::Services;

RustedTimeService::RustedTimeService(Network::Connection & connection,
                                     Common::Game::IRustedTime & time) : 
    m_connection(connection),
    m_time(time)
{
}

void RustedTimeService::synchronize()
{
    LOG_INFO << "Synchronizing time with the server\n";
    Common::Messages::RustedTimeEpochReq req;
    m_connection.send(req);
}

void RustedTimeService::handle(const Common::Messages::RustedTimeEpochResp & resp)
{
    LOG_INFO << "Current time on server: " << resp.time << "\n";
    m_time.setReferenceTime(resp.time);
}
