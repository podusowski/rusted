#pragma once

#include <set>

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
};

}
}
}
