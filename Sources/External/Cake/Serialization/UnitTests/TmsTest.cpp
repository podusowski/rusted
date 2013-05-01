#include <gtest/gtest.h>

#include "Serialization/Tms.hpp"

using namespace testing;

class TmsTest : public Test
{
protected:
    virtual void SetUp()
    {
        m_stream <<
            "variable1 1\n"
            "variable2  2\n";
    }

    std::stringstream m_stream;
};

TEST_F(TmsTest, Read)
{
    Cake::Serialization::Tms tms(m_stream);
    EXPECT_EQ(1, tms.getValue<unsigned>("variable1"));
    EXPECT_EQ(2, tms.getValue<unsigned>("variable2"));
    EXPECT_ANY_THROW(tms.getValue<unsigned>("nonexitsting_variable"));
}

TEST_F(TmsTest, CopyCtor)
{
    Cake::Serialization::Tms tms1(m_stream);
    Cake::Serialization::Tms tms2(tms1);
    EXPECT_EQ(1, tms1.getValue<unsigned>("variable1"));
}

TEST_F(TmsTest, Assign)
{
    Cake::Serialization::Tms tms1(m_stream);

    Cake::Serialization::Tms tms2 = tms1;
    EXPECT_EQ(1, tms2.getValue<unsigned>("variable1"));

    Cake::Serialization::Tms tms3;
    EXPECT_ANY_THROW(tms3.getValue<unsigned>("variable1"));
    tms3 = tms1;
    EXPECT_EQ(1, tms3.getValue<unsigned>("variable1"));
}

TEST_F(TmsTest, Has)
{
    Cake::Serialization::Tms tms1(m_stream);
    EXPECT_TRUE(tms1.has("variable1"));
    EXPECT_FALSE(tms1.has("nonexisting"));
}


