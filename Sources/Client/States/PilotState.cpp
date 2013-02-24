#include <boost/bind.hpp>
#include <OgreEntity.h>

#include "Client/States/PilotState.hpp"

using namespace Client::States;

PilotState::PilotState(IStateManagerStack &,
                       Graphics::IGraphics & graphics,
                       Client::Gui::Gui & gui, 
                       Client::Services::PlayerActionService &,
                       Views::ObjectsView & objectsView,
                       Views::PilotView & pilotView,
                       Views::ActionsView & actionsView) :
    m_graphics(graphics),
    m_gui(gui),
    m_objectsView(objectsView),
    m_pilotView(pilotView),
    m_actionsView(actionsView)
{
}

void PilotState::activate()
{
    m_gui.loadLayout("PilotScreen.layout");

    m_objectsView.activate();
    m_pilotView.activate();
}

void PilotState::deactivate()
{
}

void PilotState::frameStarted()
{
    m_objectsView.frameStarted();
    m_pilotView.frameStarted();
}

