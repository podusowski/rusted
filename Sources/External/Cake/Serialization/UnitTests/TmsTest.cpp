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

