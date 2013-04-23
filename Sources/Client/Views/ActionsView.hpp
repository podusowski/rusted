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
    void availableActionsFetched(std::vector<Common::Messages::AvailableAction>);
    void actionClicked(MyGUI::Widget *);
    void actionMouseSetFocus(MyGUI::Widget *, MyGUI::Widget * old);
    void actionMouseLostFocus(MyGUI::Widget *, MyGUI::Widget * _new);
    void disableActionButtons();
    void enableActionButtons();

    Services::PlayerActionService & m_playerActionService;
    Gui::Gui & m_gui;
    std::vector<MyGUI::Button *> m_actionButtons;
    bool m_flatActionButtons;
};

}
}
