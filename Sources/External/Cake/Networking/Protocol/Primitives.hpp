#pragma once

#include "Networking/Bytes.hpp"

#include <arpa/inet.h>
#include <stdexcept>
#include <vector>
#include <string>
#include <memory>

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
    virtual auto str() const -> std::string = 0;

    virtual ~ICodable() {}
};

class ICodableStructure : public ICodable
{
public:
    virtual auto id() const -> int = 0;

    virtual ~ICodableStructure() {}
};

class Boolean : public ICodable
{
public:
    enum size_e { size = 1 };

    Boolean(bool value = false) : m_value(value)
    {
    }

    explicit Boolean(std::string s)
    {
        std::stringstream ss(s);
        ss >> m_value;
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

    auto str() const -> std::string override
    {
        std::stringstream ss;
        ss << std::boolalpha << m_value;
        return ss.str();
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

    explicit Integer(std::string s)
    {
        std::stringstream ss(s);
        ss >> m_value;
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

    auto str() const -> std::string override
    {
        std::stringstream ss;
        ss << std::boolalpha << m_value;
        return ss.str();
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

    explicit Real(std::string s)
    {
        std::stringstream ss(s);
        float f;
        ss >> f;
        *this = f;
    }

    auto operator * () const -> float
    {
        return *m_data / 100.0;
    }

    auto operator == (const Real & other) const -> bool
    {
        return **this == *other;
    }

    auto encode() const -> Bytes override
    {
        return m_data.encode();
    }

    auto decode(const Bytes & bytes) -> std::size_t override
    {
        return m_data.decode(bytes);
    }

    auto str() const -> std::string override
    {
        std::stringstream ss;
        ss << std::boolalpha << m_data;
        return ss.str();
    }

private:
    Integer m_data;
};

inline std::ostream & operator << (std::ostream & os, const Real & value)
{
    return os << *value;
}

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
        else if (m_sizeToDecode == bytes.size())
        {
            m_value = std::string(bytes.chars(), bytes.size());
        }
        else
        {
            std::stringstream ss;
            ss << "got: " << bytes;
            throw std::runtime_error(ss.str());
        }
        return 0;
    }

    auto str() const -> std::string override
    {
        std::stringstream ss;
        ss << std::boolalpha << m_value;
        return ss.str();
    }

private:
    std::size_t m_sizeToDecode = 0;
    std::string m_value = {};
};

inline std::ostream & operator << (std::ostream & os, const String & value)
{
    return os << *value;
}

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

    Sequence(std::string s)
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

    auto operator [] (size_t index) -> T &
    {
        return m_value.at(index);
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
            T item;
            auto sizeLeft = item.decode(bytes);
            m_value.push_back(item);
            m_itemsToDecode--;

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

    auto str() const -> std::string override
    {
        std::stringstream ss;

        if (empty())
        {
            ss << "(empty)";
        }
        else for (const auto & item : *this)
        {
            ss << item.str() << " ";
        }

        return ss.str();
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

    auto size() const -> bool
    {
        return m_value.size();
    }

private:
    size_t m_itemsToDecode = 0;
    UnderlayingContainer m_value;
};

inline auto operator << (std::ostream & os, const ICodable & value) -> std::ostream &
{
    return os << value.str();
}

} // namespace Protocol
} // namespace Networking
} // namespace Cake
