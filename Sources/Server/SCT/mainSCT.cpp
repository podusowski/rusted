#include <gtest/gtest.h>
#include "Cake/Diagnostics/Logger.hpp"

int main(int argc, char ** argv)
{
    Cake::Diagnostics::Logger::getInstance().setAppName("ServerSCT");

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

