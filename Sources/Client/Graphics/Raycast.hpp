#pragma once

#include <OgreRoot.h>

namespace Client
{
namespace Graphics
{

class RaycastResult
{
public:
    bool valid;
    Ogre::Entity * entity;
    Ogre::Vector3 position;
};

class Raycast
{
public:
    Raycast(Ogre::SceneManager &);
    RaycastResult cast(const Ogre::Ray & ray);

    /* @param function This function object will be invoked for each object found across the ray.
     *                 Function should return boolean value which determinated if iteration 
     *                 should stop (false) or continue to the next object (true)
     */
    void cast(const Ogre::Ray & ray, std::function<bool(Ogre::Entity *, Ogre::Vector3)> function);

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

