#pragma once

#include "Cake/Networking/Socket.hpp"
#include "Cake/Threading/IRunnable.hpp"

#include "Network/IConnectionListener.hpp"
#include "Network/IConnection.hpp"
#include "Services/IServiceDeployment.hpp"

namespace Server
{
namespace Network
{

class Connection : public Cake::Threading::IRunnable, public Server::Network::IConnection
{
public:
	Connection(int id, Cake::Networking::Socket & socket, Services::IServiceDeployment & serviceDeployment);
	void run();
	void addListener(IConnectionListener & listener);
	void send(const Common::Messages::AbstractMessage & message);
    int getId();

private:
    int m_id;
	Cake::Networking::Socket & m_socket;
	std::vector<IConnectionListener *> m_listeners;
	std::vector<IConnectionListener *> m_listenersToAdd;
};

}
}

