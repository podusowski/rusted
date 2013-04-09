#pragma once

#include <OgreRoot.h>
#include <boost/scoped_ptr.hpp>

#include "Cake/Configuration/Configuration.hpp"

#include "Client/Graphics/Graphics.hpp"
#include "Gui/Gui.hpp"
#include "Input/Input.hpp"
#include "States/StateDeployment.hpp"
#include "Network/Connection.hpp"

namespace Client
{
namespace Engine
{

class Engine
{
public:
    Engine();
    void start();

private:
    Cake::DependencyInjection::Inject<Cake::Configuration::Configuration> m_cfg;
    boost::scoped_ptr<Graphics::Graphics> m_graphics;
    boost::scoped_ptr<Effects::Effects> m_effects;
    boost::scoped_ptr<Gui::Gui> m_gui;
    boost::scoped_ptr<Input::Input> m_input;
    boost::scoped_ptr<States::StateDeployment> m_stateDeployment;
    Network::Connection m_connection;
    Cake::Threading::Thread m_connectionThread;
};

}
}

