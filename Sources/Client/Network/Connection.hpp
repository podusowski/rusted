#pragma once

#include <queue>
#include <boost/shared_ptr.hpp>

#include "Cake/Configuration/Configuration.hpp"
#include "Cake/Threading/IRunnable.hpp"
#include "Cake/Networking/Socket.hpp"
#include "IConnection.hpp"
#include "IConnectionListener.hpp"

namespace Client
{
namespace Network
{

/**
 * @brief Manages connection to the server.
 *
 * Class is implementing listener pattern to inform about incoming messages.
 */
class Connection : public IConnection, public Cake::Threading::IRunnable
{
public:
    Connection(Cake::Configuration::Configuration & cfg);
    void run();
    void connect();
    void addListener(IConnectionListener &);
    void send(Common::Messages::AbstractMessage &);
    void yield();

private:
    Cake::Configuration::Configuration & m_cfg;
    std::shared_ptr<Cake::Networking::Socket> m_socket;
    std::vector<IConnectionListener *> m_listeners;
    std::queue<std::shared_ptr<Common::Messages::AbstractMessage>> m_messages;
};

}
}

