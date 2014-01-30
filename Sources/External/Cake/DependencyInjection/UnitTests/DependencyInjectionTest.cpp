#include <gtest/gtest.h>

#include <memory>

#include "DependencyInjection/Registry.hpp"
#include "DependencyInjection/Inject.hpp"

using namespace testing;

class IProvider
{
public:
    virtual bool foo() = 0;
    virtual ~IProvider() {}
};

class ConcreteProvider : public IProvider
{
public:
    bool foo()
    {
        return true;
    }
};

class User
{
public:
    bool doStuff()
    {
        return m_provider->foo();
    }

private:
    Cake::DependencyInjection::Inject<IProvider> m_provider;
};

TEST(DependencyInjectionTest, TestSimpleInject)
{
    Cake::DependencyInjection::Registry registry;
    registry.forInterface<IProvider>().use(std::make_shared<ConcreteProvider>());

    User user;
    EXPECT_EQ(true, user.doStuff());
}

TEST(DependencyInjectionTest, UnknownInteface)
{
    EXPECT_ANY_THROW(User user);
}

TEST(DependencyInjectionTest, ScopedRegistry)
{
    {
        Cake::DependencyInjection::Registry registry;
        registry.forInterface<IProvider>().use(std::make_shared<ConcreteProvider>());
    }

    EXPECT_ANY_THROW(User user);
}
