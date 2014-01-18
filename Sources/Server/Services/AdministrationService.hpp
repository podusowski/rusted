#pragma once

#include "Services/AbstractService.hpp"

namespace Server
{
namespace Services
{

class AdministrationService : public Server::AbstractService<AdministrationService>
{
public:
    void handle(const Common::Messages::RealoadDatabase &, Network::IConnection &);

    void handle(const Common::Messages::AbstractMessage &, Network::IConnection &) {}
};

}
}

