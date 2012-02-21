#include <boost/bind.hpp>
#include <OgreEntity.h>

#include "Client/States/PilotState.hpp"

using namespace Client::States;

PilotState::PilotState(IStateManagerStack &,
                       Graphics::IGraphics & graphics,
                       Client::Gui::Gui & gui, 
                       Client::Services::EntityService &,
                       Views::ObjectsView & objectsView,
                       Views::PilotView & pilotView) :
    m_graphics(graphics),
    m_gui(gui),
    m_objectsView(objectsView),
    m_pilotView(pilotView)
{
}

void PilotState::activate()
{
    m_gui.setLayout("PilotScreen.layout");

    m_objectsView.activate();
    m_pilotView.activate();
}

void PilotState::deactivate()
{
}

void PilotState::frameStarted()
{
    m_pilotView.frameStarted();
    m_objectsView.frameStarted();
}

