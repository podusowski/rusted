#pragma once

#include "Services/PlayerActionService.hpp"
#include "Gui/Gui.hpp"
#include "Views/IView.hpp"

namespace Client
{
namespace Views
{

class ActionsView : public IView
{
public:
    ActionsView(Services::PlayerActionService &, Gui::Gui &);
    void activate();
    void deactivate();
    void frameStarted();

private:
    Services::PlayerActionService & m_playerActionService;
    Gui::Gui m_gui;
};

}
}
