#pragma once

#include <OgreRoot.h>

namespace Client
{
namespace Graphics
{

class Raycast
{
public:
    Raycast(Ogre::SceneManager &);
    Ogre::Entity * cast(const Ogre::Ray & ray);

private:
    void getMeshInformation(
        const Ogre::MeshPtr mesh,
        size_t &vertex_count,
        Ogre::Vector3* &vertices,
        size_t &index_count,
        unsigned long* &indices,
        const Ogre::Vector3 &position,
        const Ogre::Quaternion &orient,
        const Ogre::Vector3 &scale);

    Ogre::RaySceneQuery * m_raySceneQuery;
};

}
}

