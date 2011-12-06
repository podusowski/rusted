#include "Server/Services/ServerInfoService.hpp"

using namespace Server::Services;

void ServerInfoService::handle(const Common::Messages::ServerVersionReq &, Network::IConnection & connection)
{
    Common::Messages::ServerVersionResp resp;
    resp.version = "Rusted server, development version, build "__TIME__;
    connection.send(resp);
}
