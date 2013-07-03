#include <stdexcept>

#include "Cake/Threading/ScopedLock.hpp"
#include "Common/Game/Utilities/IdGenerator.hpp"

using namespace Common::Game::Utilities;

unsigned IdGenerator::generate()
{
    Cake::Threading::ScopedLock lock(m_mutex);

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
    Cake::Threading::ScopedLock lock(m_mutex);

    auto ret = m_reserved.insert(id);

    if (!ret.second)
    {
        throw std::runtime_error("id is already reserved");
    }
}

