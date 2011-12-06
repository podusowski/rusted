#include <ctime>

#include "Time.hpp"

using namespace Cake::System;

unsigned Time::timestamp()
{
    time_t t;
    time(&t);
    return t;
}
