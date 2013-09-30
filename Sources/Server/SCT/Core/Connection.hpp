#pragma once

#include <string>
#include <tuple>
#include <type_traits>

#include "Cake/Diagnostics/Logger.hpp"
#include "Cake/Networking/Socket.hpp"

#include "Cake/Networking/Protocol/CakeReadBuffer.hpp"
#include "Common/Messages/Messages.hpp"

using ::Common::Messages::AbstractMessage;

namespace SCT
{

namespace Detail
{
    // http://stackoverflow.com/questions/7943525/is-it-possible-to-figure-out-the-parameter-type-and-return-type-of-a-lambda

    // For generic types, directly use the result of the signature of its 'operator()'
    template <typename T>
    struct function_traits : public function_traits<decltype(&T::operator())>
    {
    };

    // we specialize for pointers to member function
    template <typename ClassType, typename ReturnType, typename... Args>
    struct function_traits<ReturnType(ClassType::*)(Args...) const>
    {
        // arity is the number of arguments.
        enum { arity = sizeof...(Args) };

        typedef ReturnType result_type;

        template <size_t i> struct arg
        {
            // the i-th argument is equivalent to the i-th tuple element of a tuple
            // composed of those arguments.
            typedef typename std::tuple_element<i, std::tuple<Args...>>::type type;
        };
    };

    template<typename LambdaType>
    struct GetMessageType
    {
        typedef typename std::remove_reference<typename Detail::function_traits<LambdaType>::template arg<0>::type>::type type;
    };
}

class Connection
{
public:
    Connection(const std::string & addr, unsigned port);
    ~Connection();

    void send(::Common::Messages::AbstractMessage & message);

    template<class T> std::shared_ptr<T> receive()
    {
        LOG_INFO << "<connection:" << this << "> Waiting for " << TYPENAME(T);

        Cake::Networking::Protocol::CakeReadBuffer buffer(*m_socket);
        auto raw = Common::Messages::MessageFactory::create(buffer);

        LOG_INFO << "<connection:" << this << "> Received " << *raw;

        auto message = std::dynamic_pointer_cast<T>(raw);
        if (!message)
        {
            std::stringstream ss;
            ss << TYPENAME(*raw) << " received while " << TYPENAME(T) << " expected"; 
            LOG_ERR << "<connection:" << this << "> " << ss.str();
            throw std::runtime_error(ss.str());
        }

        return message;
    }

    template<class T1, class T2> void interleave(T1 h1, T2 h2)
    {
        LOG_INFO << "<connection:" << this << "> Waiting for " 
                 << TYPENAME(T1) 
                 << " and " 
                 << TYPENAME(T2);

        bool done[2] = { false, false };

        while (!done[0] || !done[1])
        {
            Cake::Networking::Protocol::CakeReadBuffer buffer(*m_socket);
            auto raw = Common::Messages::MessageFactory::create(buffer);

            LOG_INFO << "<connection:" << this << "> Received " << *raw;

            typedef typename Detail::GetMessageType<T1>::type Message1Type;
            typedef typename Detail::GetMessageType<T2>::type Message2Type;

            auto message1 = std::dynamic_pointer_cast<Message1Type>(raw);
            auto message2 = std::dynamic_pointer_cast<Message2Type>(raw);

            if (message1 && message2)
            {
                throw std::runtime_error("something is fucked");
            }
            else if (message1)
            {
                h1(*message1);
                done[0] = true;
            }
            else if (message2)
            {
                h2(*message2);
                done[1] = true;
            }
            else
            {
                std::stringstream ss;
                ss << TYPENAME(*raw) << " received while something else expected"; 
                LOG_ERR << "<connection:" << this << "> " << ss.str();
                throw std::runtime_error(ss.str());
            }
        }
    }

private:

    std::shared_ptr<Cake::Networking::Socket> m_socket;
    static int s_port;
};

}

