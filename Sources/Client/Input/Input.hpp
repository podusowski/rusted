#pragma once

#include <vector>

#include <OIS/OIS.h>
#include <OIS/OISMouse.h>
#include <OgreRenderWindow.h>
#include <OgreRoot.h>

#include "Graphics/Raycast.hpp"
#include "Gui/Gui.hpp"
#include "Input/IMouseListener.hpp"
#include "IInput.hpp"

namespace Client
{
namespace Input
{

class Input : public IInput, public OIS::MouseListener, public OIS::KeyListener
{
public:
    Input(Ogre::RenderWindow &, Ogre::SceneManager &, Ogre::Camera &, Client::Gui::Gui &);

    virtual bool mouseMoved( const OIS::MouseEvent &arg );
    virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
    virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
    virtual bool keyPressed( const OIS::KeyEvent &arg );
    virtual bool keyReleased( const OIS::KeyEvent &arg );

    void frameStarted();

    void addMouseListener(IMouseListener &);

    void addEntityClickedCallback(Ogre::Entity &, std::function<void()>);
    void addEntityMouseMovedCallback(Ogre::Entity & entity, std::function<void()> callback);

private:
    void mousePressedRaycast(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
    void mouseMovedRaycast();

    Ogre::Entity * raycastFromMouseCursor();

    OIS::InputManager * m_oisInputManager; 
    OIS::Mouse * m_oisMouse;
    OIS::Keyboard * m_oisKeyboard;

    std::vector<IMouseListener *> m_mouseListeners;

    Ogre::RenderWindow & m_ogreRenderWindow;
    Ogre::Camera & m_camera;

    Graphics::Raycast m_raycast;
    std::map<Ogre::Entity *, std::function<void()>> m_entityClickedCallbacks;
    std::map<Ogre::Entity *, std::function<void()>> m_entityMouseMovedCallbacks;
};

}
}

