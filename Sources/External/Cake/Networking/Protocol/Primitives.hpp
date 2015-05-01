#pragma once

#include <arpa/inet.h>
#include <stdexcept>
#include <vector>

#include "Networking/Bytes.hpp"

namespace Cake
{
namespace Networking
{
namespace Protocol
{

class ICodable
{
public:
    virtual auto encode() const -> Bytes = 0;
    virtual auto decode(const Bytes & bytes) -> size_t = 0;

    virtual ~ICodable() {}
};

class IComplex
{
public:
    virtual auto id() -> int const = 0;

    virtual ~IComplex() {}
};

class Boolean : public ICodable
{
public:
    enum size_e { size = 1 };

    Boolean(bool value = false) : m_value(value)
    {
    }

    bool operator * () const
    {
        return m_value;
    }

    auto encode() const -> Bytes override
    {
        std::uint8_t byte = m_value ? 1 : 0;
        return Bytes::from(byte);
    }

    auto decode(const Bytes & bytes) -> size_t override
    {
        if (bytes.size() == size)
        {
            m_value = bytes.as<std::uint8_t>() == 1 ? true : false;
            return 0;
        }
        else if (bytes.size() == 0)
        {
            return size;
        }
        else
        {
            std::stringstream ss;
            ss << "exactly 1 byte is required while got: " << bytes;
            throw std::runtime_error(ss.str());
        }
    }

private:
    bool m_value;
};

class Integer : public ICodable
{
public:
    using UnderlayingType = unsigned;
    enum size_e { size = sizeof(UnderlayingType) };

    Integer()
    {
    }

    Integer(UnderlayingType value) : m_value(value)
    {
    }

    Integer & operator = (UnderlayingType value)
    {
        m_value = value;
        return *this;
    }

    int operator * () const
    {
        return m_value;
    }

    bool operator == (Integer other) const
    {
        return **this == *other;
    }

    bool operator != (Integer other) const
    {
        return **this != *other;
    }

    auto encode() const -> Bytes override
    {
        return Bytes::from(htonl(m_value));
    }

    auto decode(const Bytes & bytes) -> size_t override
    {
        if (bytes.size() == size)
        {
            m_value = ntohl(bytes.as<UnderlayingType>());
            return 0;
        }
        else if (bytes.size() == 0)
        {
            return size;
        }
        else
        {
            std::stringstream ss;
            ss << "exactly 4 bytes are required while got: " << bytes;
            throw std::runtime_error(ss.str());
        }
    }

private:
    UnderlayingType m_value = {};
};

inline std::ostream & operator << (std::ostream & os, const Integer & value)
{
    return os << *value;
}

class Real : public ICodable
{
public:
    Real()
    {
    }

    Real(float value) : m_data(value * 100)
    {
    }

    auto operator * () -> float const
    {
        return *m_data / 100.0;
    }

    auto encode() const -> Bytes override
    {
        return m_data.encode();
    }

    auto decode(const Bytes & bytes) -> std::size_t override
    {
        return m_data.decode(bytes);
    }

private:
    Integer m_data;
};

class String : public ICodable
{
public:
    String()
    {
    }

    String(std::string value) : m_value(value)
    {
    }

    String & operator = (std::string value)
    {
        m_value = value;
        return *this;
    }

    std::string operator * () const
    {
        return m_value;
    }

    bool operator == (String other) const
    {
        return **this == *other;
    }

    bool operator != (String other) const
    {
        return **this != *other;
    }

    auto encode() const -> Bytes override
    {
        Bytes bytes = Integer(m_value.size()).encode();
        bytes.extend(Bytes{&m_value[0], m_value.size()});
        return bytes;
    }

    auto decode(const Bytes & bytes) -> size_t override
    {
        if (m_sizeToDecode == 0)
        {
            Integer size;
            size.decode(bytes);
            m_sizeToDecode = *size;
            return m_sizeToDecode;
        }
        else
        {
            m_value = std::string(bytes.chars(), bytes.size());
        }
        return 0;
    }

private:
    std::size_t m_sizeToDecode = 0;
    std::string m_value = {};
};

template<class T>
class Sequence : public ICodable
{
public:
    using UnderlayingContainer = std::vector<T>;

    Sequence()
    {
    }

    Sequence(std::initializer_list<T> value) : m_value(value)
    {
    }

    bool operator == (const Sequence<T> & other) const
    {
        return m_value == other.m_value;
    }

    bool operator != (const Sequence<T> & other) const
    {
        return m_value != other.m_value;
    }

    auto encode() const -> Bytes override
    {
        Bytes bytes = Integer(m_value.size()).encode();

        if (!empty())
        {
            for (const auto & i : *this)
            {
                bytes.extend(i.encode());
            }
        }

        return bytes;
    }

    auto decode(const Bytes & bytes) -> size_t override
    {
        T dummyItem;
        const auto expectedSizeByItem = dummyItem.decode(Bytes{});

        if (m_itemsToDecode == 0)
        {
            Integer size;
            size.decode(bytes);
            m_itemsToDecode = *size;

            return expectedSizeByItem;
        }
        else
        {
            m_itemsToDecode--;
            T item;
            auto sizeLeft = item.decode(bytes);
            m_value.push_back(item);

            if (sizeLeft > 0)
            {
                return sizeLeft;
            }
            else if (m_itemsToDecode > 0)
            {
                return expectedSizeByItem;
            }
            else
            {
                return 0;
            }
        }
    }

    auto begin() -> typename UnderlayingContainer::iterator
    {
        return m_value.begin();
    }

    auto begin() const -> typename UnderlayingContainer::const_iterator
    {
        return m_value.begin();
    }

    auto end() -> typename UnderlayingContainer::iterator
    {
        return m_value.end();
    }

    auto end() const -> typename UnderlayingContainer::const_iterator
    {
        return m_value.end();
    }

    auto empty() const -> bool
    {
        return m_value.empty();
    }

private:
    size_t m_itemsToDecode = 0;
    UnderlayingContainer m_value;
};

template<class T>
inline std::ostream & operator << (std::ostream & os, const Sequence<T> & value)
{
    if (value.empty())
    {
        os << "(empty)";
    }
    else for (const auto & item : value)
    {
        os << item << " ";
    }

    return os;
}

} // namespace Protocol
} // namespace Networking
} // namespace Cake
