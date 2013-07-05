#pragma once

namespace Common
{
namespace Game
{
namespace Utilities
{

template<class T> class Id
{
public:
    // TODO: this should eventually be explicit
    Id(unsigned id) : m_id(id)
    {
    }

    unsigned get()
    {
        return m_id;
    }

private:
    unsigned m_id;
};

}
}
}

