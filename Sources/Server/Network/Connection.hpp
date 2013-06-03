#pragma once

#include "Cake/Networking/Socket.hpp"
#include "Cake/Threading/IRunnable.hpp"
#include "Cake/Threading/Mutex.hpp"

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
    Connection(Cake::Networking::Socket & socket, Services::IServiceDeployment & serviceDeployment);
    ~Connection();
    void run();
    void addListener(IConnectionListener & listener);
    void send(const Common::Messages::AbstractMessage & message);

private:
    void processPendingListeners();

    Cake::Networking::Socket & m_socket;
    Cake::Threading::Mutex m_socketMutex;
    std::vector<IConnectionListener *> m_listeners;
    std::vector<IConnectionListener *> m_listenersToAdd;
};

}
}

