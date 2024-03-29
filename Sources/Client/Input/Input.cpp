#include <string>
#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>
#include <OIS/OIS.h>

#include "Cake/Diagnostics/Logger.hpp"

#include "Input/Input.hpp"

using namespace Client::Input;

Input::Input(Ogre::RenderWindow & window, Ogre::SceneManager & ogreSceneManager, Ogre::Camera & ogreCamera, Client::Gui::Gui &) : 
    m_ogreRenderWindow(window),
    m_camera(ogreCamera),
    m_raycast(ogreSceneManager),
    m_mouseFocusedEntity(nullptr)
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
    m_oisMouse->setEventCallback(this);

    m_oisKeyboard = static_cast<OIS::Keyboard *>(m_oisInputManager->createInputObject(OIS::OISKeyboard, true));
    m_oisKeyboard->setEventCallback(this);
}

bool Input::mouseMoved( const OIS::MouseEvent &arg )
{
    for (auto * listener: m_mouseListeners)
    {
        listener->mouseMoved(arg.state);
    }

    mouseMovedRaycast();
    MyGUI::InputManager::getInstance().injectMouseMove(arg.state.X.abs, arg.state.Y.abs, arg.state.Z.abs);
    return true;
}

bool Input::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
    LOG_DEBUG << "Mouse pressed (x: " << arg.state.X.abs << ", y: " << arg.state.Y.abs << ")"; 

    for (auto * listener: m_mouseListeners)
    {
        listener->mousePressed(id, arg, arg.state.X.abs, arg.state.Y.abs);
    }

    mousePressedRaycast(arg, id);
    MyGUI::InputManager::getInstance().injectMousePress(arg.state.X.abs, arg.state.Y.abs, MyGUI::MouseButton::Enum(id));
    return true;
}

bool Input::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
    LOG_DEBUG << "Mouse released (x: " << arg.state.X.abs << ", y: " << arg.state.Y.abs << ")"; 

    BOOST_FOREACH(IMouseListener * listener, m_mouseListeners)
    {
        listener->mouseReleased(id, arg.state.X.abs, arg.state.Y.abs);
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

void Input::addEntityClickedCallback(Ogre::Entity & entity, std::function<void()> callback)
{
    m_entityClickedCallbacks.insert(std::make_pair(&entity, callback));
}

void Input::addEntityMouseMovedCallback(Ogre::Entity & entity, std::function<void()> callback)
{
    m_entityMouseMovedCallbacks.insert(std::make_pair(&entity, callback));
}

void Input::addEntityMouseLeavedCallback(Ogre::Entity & entity, std::function<void()> callback)
{
    m_entityMouseLeavedCallbacks.insert(std::make_pair(&entity, callback));
}

bool Input::isShiftDown()
{
    return m_oisKeyboard->isModifierDown(OIS::Keyboard::Shift);
}

bool Input::isCtrlDown()
{
    return m_oisKeyboard->isModifierDown(OIS::Keyboard::Ctrl);
}

bool Input::isAltDown()
{
    return m_oisKeyboard->isModifierDown(OIS::Keyboard::Alt);
}

void Input::mousePressedRaycast(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
    auto * entity = raycastFromMouseCursor();
    if (entity)
    {
        auto it = m_entityClickedCallbacks.find(entity);
        if (it != m_entityClickedCallbacks.end())
        {
            it->second();
        }
    }
}

void Input::mouseMovedRaycast()
{
    auto * entity = raycastFromMouseCursor();

    mouseLeavedCallbackIfNeeded(entity);

    if (entity)
    {
        auto it = m_entityMouseMovedCallbacks.find(entity);
        if (it != m_entityMouseMovedCallbacks.end())
        {
            it->second();
        }
    }
}

void Input::mouseLeavedCallbackIfNeeded(Ogre::Entity * entity)
{
    if (entity != m_mouseFocusedEntity)
    {
        auto it = m_entityMouseLeavedCallbacks.find(m_mouseFocusedEntity);
        if (it != m_entityMouseLeavedCallbacks.end())
        {
            it->second();
        }

        m_mouseFocusedEntity = entity;
    }
}

Ogre::Entity * Input::raycastFromMouseCursor()
{
    auto mousePos = MyGUI::InputManager::getInstance().getMousePosition();
    Ogre::Ray mouseRay = m_camera.getCameraToViewportRay(
                            mousePos.left / float(m_ogreRenderWindow.getWidth()), 
                            mousePos.top / float(m_ogreRenderWindow.getHeight()));

    auto result = m_raycast.cast(mouseRay);
    return result.entity;
}

