#pragma once

#include <OgreRoot.h>
#include <boost/scoped_ptr.hpp>

#include "Common/Configuration/Configuration.hpp"

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
    Engine(Common::Configuration::Configuration & cfg);
    void start();

private:
    boost::scoped_ptr<Graphics::Graphics> m_graphics;
    boost::scoped_ptr<Gui::Gui> m_gui;
    boost::scoped_ptr<Input::Input> m_input;
    boost::scoped_ptr<States::StateDeployment> m_stateDeployment;
    Network::Connection m_connection;
    Common::Thread m_connectionThread;
    Common::Configuration::Configuration & m_cfg;
};

}
}

