#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "Networking/ServerSocketPool.hpp"
#include "Threading/Thread.hpp"

class Client : public Cake::Threading::IRunnable
{
public:
    void run()
    {
        auto socket = Cake::Networking::Socket::connectToTcpSocket("localhost", 2001);
    }
};

TEST(ServerSocketPoolTest, Accept)
{
    auto server1 = Cake::Networking::ServerSocket::createTcpServer(2000);
    auto server2 = Cake::Networking::ServerSocket::createTcpServer(2001);

    Cake::Networking::ServerSocketPool pool;
    pool.add(server1);
    pool.add(server2);

    Client client;
    Cake::Threading::Thread thread(client);
    thread.start();

    auto socketPair = pool.accept();

    EXPECT_EQ(server2, socketPair.second);
}

