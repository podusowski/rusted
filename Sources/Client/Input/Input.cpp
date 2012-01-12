#include <string>
#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>
#include <OIS/OIS.h>

#include "Cake/Diagnostics/Logger.hpp"

#include "Input/Input.hpp"

using namespace Client::Input;

Input::Input(Ogre::RenderWindow & window, Client::Gui::Gui &) : 
    m_ogreRenderWindow(window)
{
    LOG_INFO << "Initializing input subsystem";

    size_t windowHandle;
    window.getCustomAttribute("WINDOW", &windowHandle);

    OIS::ParamList oisOptions;
    std::string windowHandleStr = boost::lexical_cast<std::string>(unsigned(windowHandle));

    oisOptions.insert(OIS::ParamList::value_type("WINDOW", windowHandleStr));

    m_oisInputManager = OIS::InputManager::createInputSystem(oisOptions);

    m_oisMouse = static_cast<OIS::Mouse *>(m_oisInputManager->createInputObject(OIS::OISMouse, true));
    const OIS::MouseState & mouseState = m_oisMouse->getMouseState();
    mouseState.width = window.getWidth();
    mouseState.height = window.getHeight();
    mouseX = window.getWidth() / 2;
    mouseY = window.getHeight() / 2;
    m_oisMouse->setEventCallback(this);

    m_oisKeyboard = static_cast<OIS::Keyboard *>(m_oisInputManager->createInputObject(OIS::OISKeyboard, true));
    m_oisKeyboard->setEventCallback(this);
}

// TODO: remove depencency from CEGUI here

bool Input::mouseMoved( const OIS::MouseEvent &arg )
{
    BOOST_FOREACH(IMouseListener * listener, m_mouseListeners)
    {
        listener->mouseMoved(arg.state);
    }
    mouseX += arg.state.X.rel;
    mouseY += arg.state.Y.rel;

    if (mouseX < 0)
        mouseX = 0;
    if (mouseX > m_ogreRenderWindow.getWidth())
        mouseX = m_ogreRenderWindow.getWidth();

    if (mouseY < 0)
        mouseY = 0;
    if (mouseY > m_ogreRenderWindow.getHeight())
        mouseY = m_ogreRenderWindow.getHeight();

    CEGUI::System::getSingleton().injectMouseMove(arg.state.X.rel, arg.state.Y.rel);
    return true;
}

bool Input::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
    LOG_INFO << "Mouse pressed (x: " << mouseX << ", y: " << mouseY << ")"; 
    BOOST_FOREACH(IMouseListener * listener, m_mouseListeners)
    {
        listener->mousePressed(id, mouseX, mouseY);
    }

    CEGUI::System::getSingleton().injectMouseButtonDown(toCeguiMouseButton(id));
    return true;
}

bool Input::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
    LOG_INFO << "Mouse released (x: " << mouseX << ", y: " << mouseY << ")"; 
    BOOST_FOREACH(IMouseListener * listener, m_mouseListeners)
    {
        listener->mouseReleased(id, mouseX, mouseY);
    }

    CEGUI::System::getSingleton().injectMouseButtonUp(toCeguiMouseButton(id));
    return true;
}

bool Input::keyPressed( const OIS::KeyEvent &arg )
{
    if (arg.key == OIS::KC_ESCAPE)
    {
        LOG_WARN << "Emergency abort key - KC_ESCAPE";
        ::abort();
    }
    CEGUI::System::getSingleton().injectKeyDown(arg.key);
    CEGUI::System::getSingleton().injectChar(arg.text);
    return true;
}

bool Input::keyReleased( const OIS::KeyEvent &arg )
{
    CEGUI::System::getSingleton().injectKeyUp(arg.key);
    return true;
}

void Input::frameStarted()
{
    m_oisMouse->capture(); 
    m_oisKeyboard->capture();
}

void Input::addMouseListener(IMouseListener & listener)
{
    m_mouseListeners.push_back(&listener);
}

CEGUI::MouseButton Input::toCeguiMouseButton(OIS::MouseButtonID oisMouseButton)
{
    switch (oisMouseButton)
    {
    case OIS::MB_Left:
        return CEGUI::LeftButton;
 
    case OIS::MB_Right:
        return CEGUI::RightButton;
 
    case OIS::MB_Middle:
        return CEGUI::MiddleButton;
 
    default:
        LOG_WARN << "Wrong OIS::MouseButtonID, LeftButton assumed.";
        return CEGUI::LeftButton;
    }
}

