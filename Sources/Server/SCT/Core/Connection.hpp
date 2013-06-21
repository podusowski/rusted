#pragma once

#include <string>

#include "Cake/Diagnostics/Logger.hpp"
#include "Cake/Networking/Socket.hpp"

#include "Cake/Networking/Protocol/CakeReadBuffer.hpp"
#include "Common/Messages/Messages.hpp"

using ::Common::Messages::AbstractMessage;

namespace SCT
{

class Connection
{
public:
    Connection(const std::string & addr, unsigned port);
    ~Connection();

    void send(::Common::Messages::AbstractMessage & message);

    template<class T> boost::shared_ptr<T> receive()
    {
        LOG_INFO << "<connection:" << this << "> Waiting for " << TYPENAME(T);

        Cake::Networking::Protocol::CakeReadBuffer buffer(*m_socket);
        auto raw = Common::Messages::MessageFactory::create(buffer);

        LOG_INFO << "<connection:" << this << "> Received " << *raw;

        auto message = boost::dynamic_pointer_cast<T>(raw);
        if (!message)
        {
            std::stringstream ss;
            ss << TYPENAME(*raw) << " received while " << TYPENAME(T) << " expected"; 
            LOG_ERR << "<connection:" << this << "> " << ss.str();
            throw std::runtime_error(ss.str());
        }

        return message;
    }

private:
    boost::shared_ptr<Cake::Networking::Socket> m_socket;
    static int s_port;
};

}

