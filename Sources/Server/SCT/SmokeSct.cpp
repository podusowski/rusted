#include <gtest/gtest.h>

#include "Cake/Threading/Thread.hpp"

#include "Core/Component.hpp"
#include "Core/Connection.hpp"
#include "Core/Configuration.hpp"
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
        for (int i = 0; i < 50; i++)
        {
            std::shared_ptr<SCT::Connection> connection1 = authorizeUser(m_component, m_user, m_password);

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
        auto component = SCT::make_default_component();
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

    TEST(SmokeSct, BigDataBase)
    {
        SCT::Component component(SCT::Configuration::get_big_database_path(), SCT::Configuration::get_sut_path());
        component.start();

        std::shared_ptr<SCT::Connection> connection1;

        for (int i = 0; i < 30; i++)
        {
        try
        {
            Cake::Threading::Thread::wait(1, 0);
            connection1 = authorizeUser(component, "user1", "password");
            break;
        }
        catch(...) { }
        }

        if (!connection1)
        {
        throw std::runtime_error("can't connect");
        }

        Common::Messages::GetVisibleObjects getVisibleObjects;
        connection1->send(getVisibleObjects);

        auto visibleObjects = connection1->receive<Common::Messages::VisibleObjects>();
        EXPECT_EQ(10000u, visibleObjects->objects.size());
    }

