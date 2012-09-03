#pragma once

#include <OgreRoot.h>
#include "Common/Math/Quaternion.hpp"

namespace Client
{
namespace Graphics 
{

class IGraphics
{
public:
    virtual Ogre::SceneManager & getSceneManager() = 0;
    virtual Ogre::RenderWindow & getRenderWindow() = 0;
    virtual Ogre::Camera & getCamera() = 0;
    virtual unsigned getWidth() = 0;
    virtual unsigned getHeight() = 0;
    virtual Ogre::Quaternion toOgreQuaternion(Common::Math::Quaternion) = 0;

    virtual ~IGraphics() {}
};

}
}
