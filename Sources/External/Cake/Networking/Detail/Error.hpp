#pragma once

#include <string>

namespace Cake
{
namespace Networking
{
namespace Detail
{

class Error
{
public:
    static std::string lastError();
};

}
}
}

