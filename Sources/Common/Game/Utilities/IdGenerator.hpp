#pragma once

#include <set>
#include "Cake/Threading/Mutex.hpp"

namespace Common
{
namespace Game
{
namespace Utilities
{

class IdGenerator
{
public:
    unsigned generate();
    void reserve(unsigned id);

private:
    std::set<unsigned> m_reserved;
    Cake::Threading::Mutex m_mutex;
};

}
}
}
