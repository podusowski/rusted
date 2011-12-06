#include <gtest/gtest.h>
#include "DependencyInjection/Registry.hpp"
#include "DependencyInjection/Inject.hpp"

using namespace Cake::DependencyInjection;

class InjectTest : public testing::Test
{
public:
    void SetUp()
    {
        // this is what mean singleton pain :(
        clear();
    }
};

struct IService
{
    // need to be polimorphic for typeid to work,
    // see by yourself and comment this out
    virtual ~IService() {}
};

struct ConcreteService : public IService
{
};

struct A
{
    Inject<IService> m_service;
};

TEST_F(InjectTest, SimpleInject)
{
    forInterface<IService>()
        .use<ConcreteService>();

    A a;

    EXPECT_TRUE(typeid(*a.m_service) == typeid(ConcreteService));
}

TEST_F(InjectTest, ImplNotRegistered)
{
    boost::shared_ptr<A> a;
    EXPECT_ANY_THROW(a.reset(new A()));
}

struct ITyre { virtual ~ITyre() {} };
struct ConcreteTyre : public ITyre {};

struct IWheel { virtual ~IWheel() {} };
struct ConcreteWheel : public IWheel
{
    Inject<ITyre> m_tyre;
};

struct Car
{ 
    Inject<IWheel> m_wheel;
};

TEST_F(InjectTest, Recursive)
{
    forInterface<ITyre>()
          .use<ConcreteTyre>();

    forInterface<IWheel>()
          .use<ConcreteWheel>();

    Car car;
    EXPECT_TRUE(typeid(*car.m_wheel) == typeid(ConcreteWheel));
    ConcreteWheel & wheel = dynamic_cast<ConcreteWheel&>(*car.m_wheel);
    EXPECT_TRUE(typeid(*wheel.m_tyre) == typeid(ConcreteTyre));
}

TEST_F(InjectTest, RecursiveReverseRegistration)
{
    // those are reversed 
    forInterface<IWheel>()
          .use<ConcreteWheel>();

    forInterface<ITyre>()
          .use<ConcreteTyre>();

    Car car;
}

struct Wheel 
{
    Inject<ITyre> m_tyres[2];
};

TEST_F(InjectTest, MultipleObjectsAreIndependedInDefaultScope)
{
    forInterface<ITyre>()
          .use<ConcreteTyre>();

    Wheel wheel;

    EXPECT_TRUE(&*(wheel.m_tyres[0]) != &*(wheel.m_tyres[1]));
}

TEST_F(InjectTest, DoubleRegister)
{
    forInterface<ITyre>().use<ConcreteTyre>();
    EXPECT_ANY_THROW(forInterface<ITyre>().use<ConcreteTyre>());
}

struct TyreWithInstanceCounter : public ITyre
{
public:
    TyreWithInstanceCounter()
    {
        ++m_instances;
    }

    ~TyreWithInstanceCounter()
    {
        --m_instances;
    }

    static int m_instances;
};

int TyreWithInstanceCounter::m_instances = 0;

TEST_F(InjectTest, ImplementationIsFreedAfterHolder)
{
    forInterface<ITyre>()
          .use<TyreWithInstanceCounter>();

    {
        Wheel wheel;
    }

    EXPECT_TRUE(0 == TyreWithInstanceCounter::m_instances);
}

struct IC1 { virtual ~IC1() {} };
struct IC2 { virtual ~IC2() {} };

struct C1 : public IC1
{
    Inject<IC2> c;
};

struct C2 : public IC2
{
    Inject<IC1> c;
};

TEST_F(InjectTest, CycledInject)
{
    forInterface<IC1>().use<C1>();
    forInterface<IC2>().use<C2>();

    EXPECT_ANY_THROW(C1 c1);
}

TEST_F(InjectTest, InjectInstance)
{
    boost::shared_ptr<IService> concreteService(new ConcreteService());
    forInterface<IService>().use(concreteService);

    A a;
    EXPECT_TRUE(&(*a.m_service) == concreteService.get());
}

