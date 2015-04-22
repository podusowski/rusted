#pragma once

#include <cstddef>
#include <cstdint>
#include <cassert>
#include <vector>

namespace Cake
{
namespace Networking
{

class Bytes
{
public:
    Bytes()
    {
    }

    explicit Bytes(size_t size) : m_data(size)
    {
    }

    Bytes(const void * source, size_t size) : m_data(size)
    {
        const char * sourceChars = reinterpret_cast<const char*>(source);
        std::copy(sourceChars, sourceChars + size, chars());
    }

    Bytes(const Bytes &) = default;
    Bytes(Bytes &&) = default;
    Bytes & operator = (const Bytes &) = default;

    auto operator * () -> void *
    {
        return &m_data[0];
    }

    void extend(const Bytes & other)
    {
        auto oldSize = m_data.size();
        m_data.resize(size() + other.size());
    }

    auto chars() -> char *
    {
        return &m_data[0];
    }

    auto chars() const -> const char *
    {
        return &m_data[0];
    }

    auto some(size_t from, size_t to) const -> Bytes
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

    static auto asNetworkEndianess(std::uint8_t from) -> Bytes
    {
        return Bytes{&from, sizeof(from)};
    }

private:
    std::vector<char> m_data;
};

} // namespace Networking
} // namespace Cake
