#include "Cake/Diagnostics/Logger.hpp"

#include "Client/Engine/Engine.hpp"

using namespace Client::Engine;

Engine::Engine() :
    m_connection(*m_cfg),
    m_connectionThread(m_connection)
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
        m_effects.reset(new Visuals::Effects(*m_graphics));
        m_gui.reset(new Gui::Gui(m_graphics->getRenderWindow(), m_graphics->getSceneManager()));
        m_input.reset(new Input::Input(m_graphics->getRenderWindow(), m_graphics->getSceneManager(), m_graphics->getCamera(), *m_gui));
        m_stateDeployment.reset(new States::StateDeployment(*m_graphics, *m_effects, *m_input, *m_gui, m_connection, m_connectionThread));

        while (!m_graphics->getRenderWindow().isClosed())
        {
            m_input->frameStarted();
            m_stateDeployment->frameStarted();
            m_connection.yield();
            m_effects->frameStarted();

            if (!m_graphics->frameStarted())
            {
                break;
            }
        }
        LOG_INFO << "No further state to execute, application is going to shut down";
    }
    catch (std::exception & ex)
    {
        LOG_ERR << "Exception of type " << TYPENAME(ex) << " thrown, reason: " << ex.what();
    }
}

