#include <RustedCodec/SimpleWriteBuffer.hpp>

#include <UnitTests/ConnectionStub.hpp>

void ConnectionStub::send(Common::Messages::AbstractMessage & message)
{
    std::vector<char> messageBuffer;
    Common::RustedCodec::SimpleWriteBuffer writer(messageBuffer);
    message.serialize(writer);
    m_receivedMessages.push_back(messageBuffer);
}

void ConnectionStub::addListener(Server::Network::IConnectionListener &)
{
}

size_t ConnectionStub::getReceivedCount()
{
    return m_receivedMessages.size();
}

int ConnectionStub::getId()
{
    return 0xf00d;
}

