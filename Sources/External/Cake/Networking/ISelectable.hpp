#pragma once

namespace Cake
{
namespace Networking
{

class ISelectable
{
public:
    virtual void act() = 0;
    virtual auto nativeHandle() -> int const = 0;

    virtual ~ISelectable() {}
};

} // namespace Networking
} // namespace Cake
