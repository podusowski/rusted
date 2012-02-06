#ifndef CONNECTION_STUB_HPP
#define CONNECTION_STUB_HPP

#include <iostream>

#include <RustedCodec/SimpleReadBuffer.hpp>
#include <Network/Connection.hpp>

class ConnectionStub : public Server::Network::IConnection
{
public:
    void send(const Common::Messages::AbstractMessage & message);
    void addListener(Server::Network::IConnectionListener & listener);

    size_t getReceivedCount();

    template<typename MessageType> std::auto_ptr<MessageType> getReceivedMessage(size_t index)
    {
        Common::RustedCodec::SimpleReadBuffer reader(m_receivedMessages.at(index));
        std::auto_ptr<Common::Messages::AbstractMessage> message = Common::Messages::MessageFactory::create(reader);
        MessageType * typedMessage = dynamic_cast<MessageType *>(message.release());
        if (!typedMessage)
        {
            std::cout << "ConnectionStub::getReceivedMessage: message type not match, received msgid = "
                    << message->getId() << "\n";
            throw std::exception();
        }
        return std::auto_ptr<MessageType>(typedMessage);
    }

    int getId();

private:
    std::vector<std::vector<char> > m_receivedMessages;
};

#endif
