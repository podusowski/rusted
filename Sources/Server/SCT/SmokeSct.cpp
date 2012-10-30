#include <gtest/gtest.h>

#include "Cake/Threading/Thread.hpp"

#include "Core/Component.hpp"
#include "Core/Connection.hpp"
#include "Preconditions.hpp"
#include "UserFunctions.hpp"

using namespace Common::Messages;

class SmokeSctRunnable : public Cake::Threading::IRunnable
{
public:
    SmokeSctRunnable(SCT::Component & component, std::string user, std::string password): 
        m_component(component),
        m_user(user),
        m_password(password)
    {
    }

    void run()
    {
        for (int i = 0; i < 20; i++)
        {
            boost::shared_ptr<SCT::Connection> connection1 = authorizeUser(m_component, m_user, m_password); 

            Common::Messages::FocusObject focusObject;
            focusObject.id = 1;
            connection1->send(focusObject);

            Common::Messages::FetchAvailableActions fetchAvailableActions;
            fetchAvailableActions.shipId = 1;
            connection1->send(fetchAvailableActions);
            connection1->receive<Common::Messages::AvailableActions>();

            Common::Messages::SelectObject selectObject;
            selectObject.id = 2;
            connection1->send(selectObject);
        }
    }

private:
    SCT::Component & m_component; 
    std::string m_user;
    std::string m_password;
};

TEST(SmokeSct, Smoke)
{
	SCT::Component component("SampleDataBase.xml");
    component.start();

    SmokeSctRunnable runnable1(component, "user1", "password");
    Cake::Threading::Thread thread1(runnable1);

    SmokeSctRunnable runnable2(component, "user2", "password");
    Cake::Threading::Thread thread2(runnable2);

    thread1.start();
    thread2.start();

    thread1.join();
    thread2.join();
}

