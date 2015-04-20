#pragma once

#include <vector>

namespace Cake
{
namespace Networking
{

class Bytes
{
public:
    explicit Bytes(size_t size) : m_data(size)
    {
    }

    Bytes(const Bytes &) = default;
    Bytes(Bytes &&) = default;
    Bytes & operator = (const Bytes &) = default;

    auto operator * () -> void *
    {
        return &m_data[0];
    }

    auto chars() -> char *
    {
        return &m_data[0];
    }

    auto chars() const -> const char *
    {
        return &m_data[0];
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

private:
    std::vector<char> m_data;
};

} // namespace Networking
} // namespace Cake
