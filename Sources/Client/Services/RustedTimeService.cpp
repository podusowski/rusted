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

    Common::Messages::RustedTimeEpochReq req;
    m_connection.send(req);
}

void RustedTimeService::handle(const Common::Messages::RustedTimeEpochResp & resp)
{
    LOG_INFO << "Current time on server: " << resp.time;

    m_time->setReferenceTime(resp.time);
}
