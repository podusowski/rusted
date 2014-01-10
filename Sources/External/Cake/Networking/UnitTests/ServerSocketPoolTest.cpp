#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "Networking/ServerSocketPool.hpp"
#include "Threading/Thread.hpp"

using namespace Cake::Networking;

class Client : public Cake::Threading::IRunnable
{
public:
    Client(int port) : m_port(port)
    {
    }

    void run()
    {
        auto socket = Socket::connectToTcpSocket("localhost", m_port);
    }

private:
    int m_port;
};

TEST(ServerSocketPoolTest, Accept)
{
    std::shared_ptr<ServerSocket> server1;
    std::shared_ptr<ServerSocket> server2;

    int port = 2000;

    for (; port < 2500; port += 2)
    {
        try
        {
            server1 = ServerSocket::createTcpServer(port);
            server2 = ServerSocket::createTcpServer(port + 1);
        }
        catch (const std::exception &)
        {
            continue;
        }

        break;
    }

    Cake::Networking::ServerSocketPool pool;
    pool.add(server1);
    pool.add(server2);

    Client client(port + 1);
    Cake::Threading::Thread thread(client);
    thread.start();

    auto socketPair = pool.accept();

    EXPECT_EQ(server2, socketPair.second);
}

