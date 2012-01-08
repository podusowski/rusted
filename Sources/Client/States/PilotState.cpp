#include <boost/bind.hpp>
#include <OgreEntity.h>

#include "Common/Logger/Logger.hpp"
#include "Client/States/PilotState.hpp"

using namespace Client::States;

PilotState::PilotState(IStateManagerStack &,
                       Graphics::IGraphics & graphics,
                       Client::Gui::Gui & gui, 
                       Client::Services::EntityService &,
                       Views::StaticObjectView & staticObjectView,
                       Views::PilotView & pilotView) :
    m_graphics(graphics),
    m_gui(gui),
    m_staticObjectView(staticObjectView),
    m_pilotView(pilotView)
{
}

void PilotState::activate()
{
    m_gui.setLayout("PilotScreen.layout");

    m_staticObjectView.activate();
    m_pilotView.activate();
}

void PilotState::deactivate()
{
}

void PilotState::frameStarted()
{
    m_pilotView.frameStarted();
}

