#pragma once

#include <boost/asio.hpp>
#include <Common/IRunnable.hpp>
#include <Network/IConnectionListener.hpp>
#include <Network/IConnection.hpp>

using boost::asio::ip::tcp;

namespace Server
{
namespace Network
{

class Connection : public ::Common::IRunnable, public Server::Network::IConnection
{
public:
	Connection(int id, tcp::socket & socket);
	void run();
	void addListener(IConnectionListener & listener);
	void send(::Common::Messages::AbstractMessage & message);
    int getId();

private:
    int m_id;
	tcp::socket & m_socket;
	std::vector<IConnectionListener *> m_listeners;
	std::vector<IConnectionListener *> m_listenersToAdd;
};

}
}

