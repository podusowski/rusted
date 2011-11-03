#pragma once

#include <string>

#include "IShell.hpp"

namespace Cake 
{
namespace System
{

class Shell : public IShell
{
public:
    ShellResult execute(const std::string &);
};

}
}
