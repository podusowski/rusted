#pragma once

#include <functional>

#include "Client/Input/IMouseListener.hpp"

namespace Client
{
namespace Input
{

class IInput
{
public:
    virtual void addMouseListener(IMouseListener &) = 0;
    virtual void addObjectRightClickCallback(Ogre::Entity &, std::function<void()>) = 0;
};

}
}
