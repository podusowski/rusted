#include <gtest/gtest.h>

#include "DependencyInjection/Registry.hpp"
#include "DependencyInjection/Inject.hpp"

using namespace testing;

template<int N> struct Interface
{
    virtual ~Interface() {}
};

template<int N> struct Implementation : public Interface<N>
{
};

template<int N> void registerInterface(Cake::DependencyInjection::Registry & r)
{
    registerInterface<N - 1>(r);
    r.forInterface<Interface<N>>().template use<Implementation<N>>();
}

template<> void registerInterface<0>(Cake::DependencyInjection::Registry &)
{
}

TEST(ProfileTest, Smoke)
{
    Cake::DependencyInjection::Registry r;
    registerInterface<100>(r);
}

