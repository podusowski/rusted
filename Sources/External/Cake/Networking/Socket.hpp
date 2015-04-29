#pragma once

#include "ISelectable.hpp"
#include "Bytes.hpp"

#include <vector>
#include <map>
#include <string>
#include <memory>
#include <boost/utility.hpp>

namespace Cake
{
namespace Networking
{

class Socket : public boost::noncopyable, public ISelectable
{
public:
    using DataReceived = std::function<void(void*)>;

    Socket(int sockFd);
    ~Socket();

    static auto connectToUnixSocket(const std::string & path) -> std::shared_ptr<Socket>;
    static auto connectToTcpSocket(const std::string & address, int port) -> std::shared_ptr<Socket>;

    void send(const Bytes &);
    auto receive(size_t) -> Bytes;

    void asyncReceive(size_t, DataReceived);

    // TODO: those should be moved somewhere outside this class
    Socket & operator>>(unsigned &);
    Socket & operator<<(unsigned);

    Socket & operator>>(std::string &);
    Socket & operator<<(const std::string &);

    typedef std::map<std::string, std::string> StringMap;
    Socket & operator>>(StringMap &);
    Socket & operator<<(const StringMap &);

    void act() override;
    auto nativeHandle() -> int const override;

private:
    int m_sockFd;

    using Buffer = std::vector<char>;

    Buffer m_buffer;
    size_t m_expecting = 0;
    DataReceived m_dataReceived;
};

}
}
