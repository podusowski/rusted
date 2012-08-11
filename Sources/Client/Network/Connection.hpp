#pragma once

#include <queue>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>

#include "Cake/Configuration/Configuration.hpp"
#include "Cake/Threading/IRunnable.hpp"
#include "IConnection.hpp"
#include "IConnectionListener.hpp"

using boost::asio::ip::tcp;

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
    boost::asio::io_service io_service;
    tcp::socket m_socket;
    std::vector<IConnectionListener *> m_listeners;
    std::queue<boost::shared_ptr<Common::Messages::AbstractMessage>> m_messages;
};

}
}

