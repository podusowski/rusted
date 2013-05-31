#pragma once

#include <string>

#include "Cake/Diagnostics/Logger.hpp"
#include "Cake/Networking/Socket.hpp"

#include "Common/RustedCodec/CakeReadBuffer.hpp"
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
        LOG_INFO << "Waiting for " << TYPENAME(T);

        Common::RustedCodec::CakeReadBuffer buffer(*m_socket);
        boost::shared_ptr<T> message;

        try
        {
            auto raw = Common::Messages::MessageFactory::create(buffer);
            message = boost::dynamic_pointer_cast<T>(raw);
            if (!message)
            {
                std::stringstream ss;
                ss << TYPENAME(*raw) << " received while " << TYPENAME(T) << " expected"; 
                throw std::runtime_error(ss.str());
            }
        }
        catch (...)
        {
            LOG_INFO << "Server droped the connection";
            throw;
        }

        LOG_INFO << "Received " << *message;

        return message;
    }

private:
    boost::shared_ptr<Cake::Networking::Socket> m_socket;
    static int s_port;
};

}

