#pragma once

#include <string>

namespace Common
{
namespace Game
{
namespace Utilities
{

class PasswordHash
{
public:
    std::string generate(std::string password);
};

}
}
}

