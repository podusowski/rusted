#pragma once

#include "Client/Network/IConnectionListener.hpp"

namespace Client
{
namespace Services
{

template <typename ServiceType>
class AbstractService : public Network::IConnectionListener
{
public:
    // NOTE: ServiceType isn't constructed yet, but HandlerCaller is lazy, it doesn't do anything
    //       with service until message arrive
    AbstractService() : m_caller(reinterpret_cast<ServiceType&>(*this)) {}

    virtual void messageReceived(Common::Messages::AbstractMessage & message)
    {
        m_caller.call(message);
    }

private:
    Common::Messages::HandlerCaller0<ServiceType> m_caller;
};

}
}
