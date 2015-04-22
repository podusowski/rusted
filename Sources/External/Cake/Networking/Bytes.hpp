#pragma once

#include <cstddef>
#include <cstdint>
#include <cassert>
#include <vector>
#include <ostream>
#include <iomanip>

namespace Cake
{
namespace Networking
{

class Bytes
{
public:
    using Byte = std::uint8_t;

    Bytes()
    {
    }

    explicit Bytes(size_t size) : m_data(size)
    {
    }

    Bytes(const void * source, size_t size) : m_data(size)
    {
        const auto * sourceChars = reinterpret_cast<const Byte *>(source);
        std::copy(sourceChars, sourceChars + size, chars());
    }

    Bytes(const Bytes &) = default;
    Bytes(Bytes &&) = default;
    Bytes & operator = (const Bytes &) = default;

    void extend(const Bytes & other)
    {
        auto oldSize = m_data.size();
        m_data.resize(size() + other.size());

        std::copy(other.chars(), other.chars() + other.size(), chars() + oldSize);
    }

    auto raw() -> Byte *
    {
        return &m_data[0];
    }

    auto raw() const -> const Byte *
    {
        return &m_data[0];
    }

    auto chars() -> char *
    {
        return reinterpret_cast<char *>(&m_data[0]);
    }

    auto chars() const -> const char *
    {
        return reinterpret_cast<const char *>(&m_data[0]);
    }

    auto fromTo(size_t from, size_t to) const -> Bytes
    {
        assert(from < to);

        Bytes bytes{to - from};

        std::copy(chars() + from, chars() + to, bytes.chars());

        return bytes;
    }

    void copyTo(void * destination) const
    {
        std::copy(chars(), chars() + size(), static_cast<char*>(destination));
    }

    auto size() const -> size_t
    {
        return m_data.size();
    }

    template<class T>
    auto as() const -> T
    {
        return *reinterpret_cast<const T*>(chars());
    }

    template<class T>
    static auto from(const T & from) -> Bytes
    {
        return Bytes(&from, sizeof(from));
    }

private:
    std::vector<Byte> m_data;
};

inline std::ostream & operator << (std::ostream & os, const Bytes & bytes)
{
    if (bytes.size() > 0)
    {
        os << bytes.size() << " bytes: ";

        const auto * raw = bytes.raw();
        for (size_t i = 0; i < bytes.size(); i++)
        {
            os << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(raw[i]) << " ";
        }
    }
    else
    {
        os << "empty";
    }

    return os;
}

} // namespace Networking
} // namespace Cake
