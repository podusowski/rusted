#pragma once

#include <stdexcept>

#include "Cake/Diagnostics/Logger.hpp"

namespace Common
{
namespace Game
{
namespace Utilities
{


/*
 * This kind of id cannot be invalid. This allows it to be LessComparable
 */
template<class T> class StrictId
{
public:
    StrictId(unsigned id) : m_id(id)
    {
    }

    unsigned get() const
    {
        return m_id;
    }

    bool operator < (const StrictId<T> other) const
    {
        return m_id < other.m_id;
    }

    bool operator == (const StrictId<T> other) const
    {
        return m_id == other.m_id;
    }

private:
    unsigned m_id;
};


template<class T> class Id
{
public:
    Id() : m_id(0), m_valid(false)
    {
    }

    Id(const Id<T> & other) : m_id(other.m_id), m_valid(other.m_id)
    {
    }

    // TODO: this should eventually be explicit
    Id(unsigned id) : m_id(id), m_valid(true)
    {
    }

    Id(const StrictId<T> id) : m_id(id.get()), m_valid(true)
    {
    }

    unsigned get() const
    {
        if (m_valid)
        {
            return m_id;
        }
        else
        {
            throw std::runtime_error("this id is invalid");
        }
    }

    bool valid() const
    {
        return m_valid;
    }

    bool operator == (const Id<T> other) const
    {
        return get() == other.get();
    }

private:
    unsigned m_id;
    bool m_valid;
};

}
}
}

template<class T> std::ostream & operator << (std::ostream & os, const Common::Game::Utilities::Id<T> id)
{
    if (id.valid())
    {
        os << id.get();
    }
    else
    {
        os << "invalid";
    }
    return os;
}

template<class T> std::ostream & operator << (std::ostream & os, const Common::Game::Utilities::StrictId<T> id)
{
    return os << id.get();
}

