#include <OgreWindowEventUtilities.h>

#include "Cake/Diagnostics/Logger.hpp"

#include "Client/Gui/CEGUIIncludes.hpp"
#include "Client/Engine/Engine.hpp"

using namespace Client::Engine;

Engine::Engine(Common::Configuration::Configuration & cfg) : 
    m_connection(cfg),
    m_connectionThread(m_connection),
    m_cfg(cfg)
{
}

// TODO: the idea for this is to move everything into Application which implement some kind of
//       IMainLoop iface in which frame listeners can be registered. Also messagePump and renderOneFrame
//       ogre stuff shall be moved into Graphics.
void Engine::start()
{
    try
    {
        m_graphics.reset(new Graphics::Graphics());
        m_gui.reset(new Gui::Gui(m_graphics->getRenderWindow(), m_graphics->getSceneManager()));
        m_input.reset(new Input::Input(m_graphics->getRenderWindow(), m_graphics->getSceneManager(), m_graphics->getCamera(), *m_gui));
        m_stateDeployment.reset(new States::StateDeployment(*m_graphics, *m_input, *m_gui, m_connection, m_connectionThread));

        while (!m_graphics->getRenderWindow().isClosed())
        {
            m_input->frameStarted();
            m_stateDeployment->frameStarted();
            m_connection.yield();

            m_graphics->getRenderWindow().swapBuffers(true);
            Ogre::WindowEventUtilities::messagePump();

            if (!m_graphics->getOgreRoot().renderOneFrame()) break;
        }
        LOG_INFO << "No further state to execute, application is going to shut down";
    }
    catch (CEGUI::Exception & ex)
    {
        LOG_ERR << "Exception of type " << TYPENAME(ex) << " thrown, reason: " << ex.getMessage();
    }
    catch (std::exception & ex)
    {
        LOG_ERR << "Exception of type " << TYPENAME(ex) << " thrown, reason: " << ex.what();
    }
}

