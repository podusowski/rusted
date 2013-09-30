#pragma once

#include "Core/Component.hpp"
#include "Core/Connection.hpp"

namespace SCT
{

class PreconditionPlayerLoggedIn
{
public:
    PreconditionPlayerLoggedIn()
    {
        using namespace Common::Messages;

        std::string dbFile = "SampleDataBase.xml";

        m_component.reset(new Component()); 
        SCT::Component & component = *m_component;
        component.setConfigValue("--database.provider", "xml");
        component.setConfigValue("--database.xml.filename", dbFile);
        component.start();
        m_connection = component.createConnection();

        UserAuthorizationReq userAuthorizationReq;
        userAuthorizationReq.login = "user1";
        userAuthorizationReq.password = "password";
        m_connection->send(userAuthorizationReq);

        m_connection->receive<UserAuthorizationResp>();
    }

    SCT::Component & getComponent() { return *m_component; }
    SCT::Connection & getConnection() { return *m_connection; }

private:
    std::shared_ptr<SCT::Component> m_component;
    std::shared_ptr<SCT::Connection> m_connection;
};

}
