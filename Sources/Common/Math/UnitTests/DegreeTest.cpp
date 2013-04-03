#include <gtest/gtest.h>
#include "Common/Math/Degree.hpp"

TEST(DegreeTest, Normalize)
{
    Common::Math::Degree d(0);
    d.normalize();
    EXPECT_EQ(0, *d);

    d = 180;
    d.normalize();
    EXPECT_EQ(180, *d);

    d = -180;
    d.normalize();
    EXPECT_EQ(-180, *d);

    d = 181;
    EXPECT_EQ(181, *d);
    d.normalize();
    EXPECT_EQ(-179, *d);

    d = 200;
    d.normalize();
    EXPECT_EQ(-160, *d);

    d = -181;
    EXPECT_EQ(-181, *d);
    d.normalize();
    EXPECT_EQ(179, *d);
}

