#pragma once

#include <functional>

#include <OgreRoot.h>
#include "Client/Input/IMouseListener.hpp"

namespace Client
{
namespace Input
{

class IInput
{
public:
    virtual void addMouseListener(IMouseListener &) = 0;
    virtual void addEntityClickedCallback(Ogre::Entity &, std::function<void()>) = 0;
    virtual void addEntityMouseMovedCallback(Ogre::Entity & entity, std::function<void()> callback) = 0;
    virtual void addEntityMouseLeavedCallback(Ogre::Entity & entity, std::function<void()> callback) = 0;
    virtual bool isShiftDown() = 0;
    virtual bool isCtrlDown() = 0;
    virtual bool isAltDown() = 0;

    virtual ~IInput() {}
};

}
}
