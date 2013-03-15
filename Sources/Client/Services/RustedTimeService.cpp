#include "Cake/Diagnostics/Logger.hpp"

#include "Client/Services/RustedTimeService.hpp"

using namespace Client::Services;

RustedTimeService::RustedTimeService(Network::IConnection & connection) : 
    m_connection(connection)
{
}

void RustedTimeService::synchronize()
{
    LOG_INFO << "Synchronizing time with the server";

    m_syncRequestTime = m_time->getAbsoluteTime();

    Common::Messages::RustedTimeEpochReq req;
    m_connection.send(req);
}

void RustedTimeService::handle(const Common::Messages::RustedTimeEpochResp & resp)
{
    Common::Game::TimeValue time(resp.seconds, resp.miliSeconds);

    LOG_INFO << "Current time on server: " << time;
    LOG_DEBUG << "Delay to server: " << m_time->getAbsoluteTime() - m_syncRequestTime;

    m_time->setReferenceTime(time);
}
