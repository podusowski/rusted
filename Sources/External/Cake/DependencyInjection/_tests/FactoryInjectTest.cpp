#include <gtest/gtest.h>

#include "DependencyInjection/Registry.hpp"
#include "DependencyInjection/Inject.hpp"

using namespace Cake::DependencyInjection;

class FactoryInjectTest : public testing::Test
{
public:
    void SetUp()
    {
        Cake::DependencyInjection::clear();
    }
};

struct IService
{
    virtual ~IService() {}
    virtual int getI() = 0;
};

struct Service : public IService
{
    Service(int i) : m_i(i) {}
    int getI() { return m_i; }
private:
    int m_i;
};

struct ServiceFactory : public Factory<IService, int>
{
    boost::shared_ptr<IService> create(int i)
    {
        return boost::shared_ptr<IService>(new Service(i));
    }
};

struct Application
{
    Application(int i) : m_service(i) {}
    Inject<IService, int> m_service;
};

TEST_F(FactoryInjectTest, SimpleInject)
{
    forInterface<IService>()
          .useFactory<ServiceFactory>();

    Application a(5);

    EXPECT_TRUE(5 == a.m_service->getI());
}

struct ServiceFactoryWithBadType : public Factory<IService, const std::string &>
{
    boost::shared_ptr<IService> create(const std::string &)
    {
        return boost::shared_ptr<IService>(new Service(5));
    }
};

TEST_F(FactoryInjectTest, FactoryHasDifferentType)
{
    forInterface<IService>()
          .useFactory<ServiceFactoryWithBadType>();

    EXPECT_ANY_THROW(Application a(5));
}

TEST_F(FactoryInjectTest, DefaultFactory)
{
    forInterface<IService>()
          .useFactory<GenericFactory1<IService, Service, int> >();

    Application a(5);

    EXPECT_TRUE(5 == a.m_service->getI());
}

TEST_F(FactoryInjectTest, DefaultFactoryWithBadType)
{
    forInterface<IService>()
          .useFactory<GenericFactory1<IService, Service, float> >();

    EXPECT_ANY_THROW(Application a(5));
}
