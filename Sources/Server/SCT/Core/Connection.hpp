#pragma once

#include <string>
#include <boost/asio.hpp>

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
	boost::shared_ptr<AbstractMessage> receive();
private:
	::boost::asio::ip::tcp::socket * m_socket;
	::boost::asio::io_service io_service;
    static int s_port;
};

}

