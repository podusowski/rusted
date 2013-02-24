#include "ActionsView.hpp"

using namespace Client::Views;

ActionsView::ActionsView(Services::PlayerActionService & playerActionService, Gui::Gui & gui) :
    m_playerActionService(playerActionService),
    m_gui(gui)
{
}

void ActionsView::activate()
{
}
void ActionsView::deactivate()
{
}
void ActionsView::frameStarted()
{
}
