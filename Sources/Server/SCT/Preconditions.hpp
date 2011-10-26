#pragma once

#include "Server/SCT/Component.hpp"
#include "Server/SCT/Connection.hpp"

namespace SCT
{

class PreconditionPlayerLoggedIn
{
public:
    PreconditionPlayerLoggedIn()
    {
        using namespace Common::Messages;

        SCT::DataBaseUtils dbUtils;
        std::string dbFile = dbUtils.createOnePlayerWithOneEntity();

        m_component.reset(new Component()); 
        SCT::Component & component = *m_component;
        component.setConfigValue("--database.provider", "xml");
        component.setConfigValue("--database.xml.filename", dbFile);
        component.start();
        SCT::Connection & connection = component.createConnection();
        m_connection = &connection;

        UserAuthorizationReq userAuthorizationReq;
        userAuthorizationReq.login = "user1";
        userAuthorizationReq.password = "password";
        connection.send(userAuthorizationReq);

        std::auto_ptr<AbstractMessage> userAuthorizationResp = connection.receive();
        assert(Id::UserAuthorizationResp == userAuthorizationResp->getId());
    }

    SCT::Component & getComponent() { return *m_component; }
    SCT::Connection & getConnection() { return *m_connection; }

private:
    std::auto_ptr<SCT::Component> m_component;
    SCT::Connection * m_connection;
};

}
