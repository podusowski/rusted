#pragma once

#include <string>

#include "ShellResult.hpp"

namespace Cake 
{
namespace System
{

class IShell
{
public:
    virtual ShellResult execute(const std::string & cmd) = 0;

    virtual ~IShell() {}
};

}
}
