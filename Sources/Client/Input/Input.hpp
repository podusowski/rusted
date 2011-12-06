#pragma once

#include <vector>

#include <OIS/OIS.h>
#include <OIS/OISMouse.h>
#include <OgreRenderWindow.h>
#include <OgreRoot.h>

#include "Gui/CEGUIIncludes.hpp"
#include "Common/Logger/Logger.hpp"
#include "Gui/Gui.hpp"
#include "Client/Input/IMouseListener.hpp"

namespace Client
{
namespace Input
{

class Input : public OIS::MouseListener, public OIS::KeyListener
{
public:
    Input(Ogre::RenderWindow &, Client::Gui::Gui &);

    virtual bool mouseMoved( const OIS::MouseEvent &arg );
    virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
    virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
    virtual bool keyPressed( const OIS::KeyEvent &arg );
    virtual bool keyReleased( const OIS::KeyEvent &arg );

    void frameStarted();
    void addMouseListener(IMouseListener &);

private:
    CEGUI::MouseButton toCeguiMouseButton(OIS::MouseButtonID oisMouseButton);

    OIS::InputManager * m_oisInputManager; 
    OIS::Mouse * m_oisMouse;
    OIS::Keyboard * m_oisKeyboard;

    std::vector<IMouseListener *> m_mouseListeners;

    Ogre::RenderWindow & m_ogreRenderWindow;

    int mouseX;
    int mouseY;
};

}
}

