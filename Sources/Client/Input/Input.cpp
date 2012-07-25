#include <string>
#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>
#include <OIS/OIS.h>

#include "Cake/Diagnostics/Logger.hpp"

#include "Input/Input.hpp"

using namespace Client::Input;

Input::Input(Ogre::RenderWindow & window, Ogre::SceneManager & ogreSceneManager, Ogre::Camera & ogreCamera, Client::Gui::Gui &) : 
    m_ogreRenderWindow(window),
    m_ogreObjectRaycaster(ogreSceneManager, ogreCamera)
{
    LOG_INFO << "Initializing input subsystem";

    size_t windowHandle;
    window.getCustomAttribute("WINDOW", &windowHandle);

    OIS::ParamList oisOptions;
    std::string windowHandleStr = boost::lexical_cast<std::string>(unsigned(windowHandle));

    oisOptions.insert(OIS::ParamList::value_type("WINDOW", windowHandleStr));

#if defined OIS_WIN32_PLATFORM
    oisOptions.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND" )));
    oisOptions.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE")));
    oisOptions.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_FOREGROUND")));
    oisOptions.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_NONEXCLUSIVE")));
#elif defined OIS_LINUX_PLATFORM
    oisOptions.insert(std::make_pair(std::string("x11_mouse_grab"), std::string("false")));
    oisOptions.insert(std::make_pair(std::string("x11_mouse_hide"), std::string("false")));
    oisOptions.insert(std::make_pair(std::string("x11_keyboard_grab"), std::string("false")));
    oisOptions.insert(std::make_pair(std::string("XAutoRepeatOn"), std::string("true")));
#endif

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

    addMouseListener(m_ogreObjectRaycaster);
}

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
    if (abs(mouseX) > m_ogreRenderWindow.getWidth())
        mouseX = m_ogreRenderWindow.getWidth();

    if (mouseY < 0)
        mouseY = 0;
    if (abs(mouseY) > m_ogreRenderWindow.getHeight())
        mouseY = m_ogreRenderWindow.getHeight();

    MyGUI::InputManager::getInstance().injectMouseMove(arg.state.X.abs, arg.state.Y.abs, arg.state.Z.abs);
    return true;
}

bool Input::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
    LOG_DEBUG << "Mouse pressed (x: " << mouseX << ", y: " << mouseY << ")"; 

    BOOST_FOREACH(IMouseListener * listener, m_mouseListeners)
    {
        listener->mousePressed(id, arg, mouseX, mouseY);
    }

    MyGUI::InputManager::getInstance().injectMousePress(arg.state.X.abs, arg.state.Y.abs, MyGUI::MouseButton::Enum(id));
    return true;
}

bool Input::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
    LOG_DEBUG << "Mouse released (x: " << mouseX << ", y: " << mouseY << ")"; 

    BOOST_FOREACH(IMouseListener * listener, m_mouseListeners)
    {
        listener->mouseReleased(id, mouseX, mouseY);
    }

    MyGUI::InputManager::getInstance().injectMouseRelease(arg.state.X.abs, arg.state.Y.abs, MyGUI::MouseButton::Enum(id));
    return true;
}

bool Input::keyPressed(const OIS::KeyEvent & arg)
{
    if (arg.key == OIS::KC_ESCAPE)
    {
        LOG_WARN << "KC_ESCAPE, abort";
        ::abort();
    }

    MyGUI::InputManager::getInstance().injectKeyPress(MyGUI::KeyCode::Enum(arg.key), arg.text);

    return true;
}

bool Input::keyReleased(const OIS::KeyEvent & arg)
{
    MyGUI::InputManager::getInstance().injectKeyRelease(MyGUI::KeyCode::Enum(arg.key));
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

void Input::addObjectRightClickCallback(Ogre::Entity & entity, std::function<void()> callback)
{
    m_ogreObjectRaycaster.addObjectRightClickCallback(entity, callback);
}

