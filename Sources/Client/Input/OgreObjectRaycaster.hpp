#include <functional>
#include <map>

#include "IMouseListener.hpp"

namespace Client
{
namespace Input
{

class OgreObjectRaycaster : public IMouseListener
{
public:
    OgreObjectRaycaster(Ogre::SceneManager &, Ogre::Camera &);
    void addObjectRightClickCallback(Ogre::Entity & entity, std::function<void()> callback);

    void mouseMoved(const OIS::MouseState &);
    void mousePressed(const OIS::MouseButtonID &, const OIS::MouseEvent & arg, unsigned x, unsigned y);
    void mouseReleased(const OIS::MouseButtonID &, unsigned x, unsigned y);

private:
    std::map<Ogre::Entity *, std::function<void()>> m_rightClickCallbacks;
    Ogre::RaySceneQuery * m_raySceneQuery;
    Ogre::Camera & m_camera;
};

}
}
