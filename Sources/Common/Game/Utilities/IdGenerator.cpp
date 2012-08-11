#include <stdexcept>

#include "Common/Game/Utilities/IdGenerator.hpp"

using namespace Common::Game::Utilities;

unsigned IdGenerator::generate()
{
    unsigned ret = 1;

    while (m_reserved.find(ret) != m_reserved.end())
    {
        ++ret;
    }

    m_reserved.insert(ret);

    return ret;
}

void IdGenerator::reserve(unsigned id)
{
    auto ret = m_reserved.insert(id);

    if (!ret.second)
    {
        throw std::runtime_error("id is reserved");
    }
}

