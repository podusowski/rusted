#include <gtest/gtest.h>

#include "Serialization/Fc.hpp"

using namespace testing;

void basicExpectations_noParam(const Cake::Serialization::Fc & fc)
{
    EXPECT_EQ("Foo", fc.getName());

    auto parameters = fc.getParameters();
    ASSERT_TRUE(parameters.empty());
}

void basicExpectations_1param(const Cake::Serialization::Fc & fc)
{
    EXPECT_EQ("Foo", fc.getName());

    auto parameters = fc.getParameters();
    ASSERT_EQ(1u, parameters.size());
    EXPECT_EQ("1", parameters[0]);
}

void basicExpectations_2params(const Cake::Serialization::Fc & fc)
{
    EXPECT_EQ("Foo", fc.getName());

    auto parameters = fc.getParameters();
    ASSERT_EQ(2u, parameters.size());
    EXPECT_EQ("1", parameters[0]);
    EXPECT_EQ("2", parameters[1]);
}

TEST(FcTest, Simple)
{
    EXPECT_NO_THROW(basicExpectations_noParam(Cake::Serialization::Fc("Foo()")));
    EXPECT_NO_THROW(basicExpectations_noParam(Cake::Serialization::Fc("Foo( )")));
    EXPECT_NO_THROW(basicExpectations_noParam(Cake::Serialization::Fc("Foo ( )")));

    EXPECT_NO_THROW(basicExpectations_1param(Cake::Serialization::Fc("Foo(1)")));
    EXPECT_NO_THROW(basicExpectations_1param(Cake::Serialization::Fc(" Foo(1)")));
    EXPECT_NO_THROW(basicExpectations_1param(Cake::Serialization::Fc(" Foo (1)")));
    EXPECT_NO_THROW(basicExpectations_1param(Cake::Serialization::Fc(" Foo( 1)")));
    EXPECT_NO_THROW(basicExpectations_1param(Cake::Serialization::Fc(" Foo ( 1)")));
    EXPECT_NO_THROW(basicExpectations_1param(Cake::Serialization::Fc(" Foo ( 1 )")));
    EXPECT_NO_THROW(basicExpectations_1param(Cake::Serialization::Fc(" Foo ( 1 ) ")));

    EXPECT_NO_THROW(basicExpectations_2params(Cake::Serialization::Fc("Foo(1,2)")));
    EXPECT_NO_THROW(basicExpectations_2params(Cake::Serialization::Fc("Foo(1, 2)")));
}

TEST(FcTest, ParseErrors)
{
    EXPECT_ANY_THROW(Cake::Serialization::Fc("F"));
    EXPECT_ANY_THROW(Cake::Serialization::Fc("F "));
    EXPECT_ANY_THROW(Cake::Serialization::Fc("F("));
    EXPECT_ANY_THROW(Cake::Serialization::Fc("F ("));
    EXPECT_ANY_THROW(Cake::Serialization::Fc("F(,"));
    EXPECT_ANY_THROW(Cake::Serialization::Fc("F(,)"));
    EXPECT_ANY_THROW(Cake::Serialization::Fc("F( , )"));
    EXPECT_ANY_THROW(Cake::Serialization::Fc("F(1,)"));
    EXPECT_ANY_THROW(Cake::Serialization::Fc("F(,1)"));
    EXPECT_ANY_THROW(Cake::Serialization::Fc("("));
    EXPECT_ANY_THROW(Cake::Serialization::Fc(")"));
    EXPECT_ANY_THROW(Cake::Serialization::Fc(","));
}


