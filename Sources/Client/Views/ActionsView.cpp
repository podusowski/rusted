#include <set>

#include "Services/PlayerActionService.hpp"

#include "ActionsView.hpp"

using namespace Client::Views;

ActionsView::ActionsView(Services::PlayerActionService & playerActionService, Gui::Gui & gui) :
    m_playerActionService(playerActionService),
    m_gui(gui),
    m_flatActionButtons(false)
{
}

void ActionsView::activate()
{
    m_playerActionService.addGlobalCooldownActivatedSlot(boost::bind(&ActionsView::disableActionButtons, this));
    m_playerActionService.addGlobalCooldownExpiredSlot(boost::bind(&ActionsView::enableActionButtons, this));
    m_playerActionService.addAvailableActionsFetchedSlot(boost::bind(&ActionsView::availableActionsFetched, this, _1));
    m_playerActionService.fetchAvailableActions();
}

void ActionsView::deactivate()
{
}

void ActionsView::frameStarted()
{
}

void ActionsView::actionClicked(MyGUI::Widget * widget)
{
    LOG_DEBUG << "Action clicked";

    auto * action = widget->getUserData<Common::Messages::AvailableAction>();
    m_playerActionService.executeAction(action->id, action->parameter);
}

void ActionsView::actionMouseSetFocus(MyGUI::Widget * sender, MyGUI::Widget * old)
{
    auto position = sender->getAbsolutePosition();
    position += MyGUI::IntPoint(60, 0);
    auto focusedActionTextBox = m_gui->findWidget<MyGUI::TextBox>("FocusedActionTextBox");
    focusedActionTextBox->setPosition(position); 

    auto * action = sender->getUserData<Common::Messages::AvailableAction>();

    std::stringstream ss;
    ss << action->description;

    focusedActionTextBox->setCaption(ss.str());
    focusedActionTextBox->setVisible(true);
}

void ActionsView::actionMouseLostFocus(MyGUI::Widget *, MyGUI::Widget * _new)
{
    m_gui->findWidget<MyGUI::TextBox>("FocusedActionTextBox")->setVisible(false);
}

void ActionsView::disableActionButtons()
{
    for (auto * button: m_actionButtons)
    {
        button->setEnabled(false);
    }
}

void ActionsView::enableActionButtons()
{
    for (auto * button: m_actionButtons)
    {
        button->setEnabled(true);
    }
}

void ActionsView::availableActionsFetched(std::vector<Common::Messages::AvailableAction> actions)
{
    auto * actionsPanel = m_gui->findWidget<MyGUI::Widget>("ActionsPanel");

    LOG_DEBUG << "Got actions";

    // delete action buttons
    for (auto & actionButton: m_actionButtons)
    {
        m_gui->destroyWidget(actionButton);
    }
    m_actionButtons.clear();

    if (m_flatActionButtons)
    {
        int buttonTop = 0;
        for (auto & action: actions)
        {
            LOG_DEBUG << "  " << action.id << "/" << action.parameter;

            auto * actionButton = actionsPanel->createWidget<MyGUI::Button>("Button", MyGUI::IntCoord(0, buttonTop, 50, 50), MyGUI::Align::Default);
            m_actionButtons.push_back(actionButton);
            buttonTop += 50;

            actionButton->setCaption(action.name);
            actionButton->setUserData(action);
            actionButton->eventMouseButtonClick += MyGUI::newDelegate(this, &ActionsView::actionClicked);
            actionButton->eventMouseSetFocus += MyGUI::newDelegate(this, &ActionsView::actionMouseSetFocus);
            actionButton->eventMouseLostFocus += MyGUI::newDelegate(this, &ActionsView::actionMouseLostFocus);
        }
    }
    else
    {
        int buttonTop = 0;
        int buttonLeft = 0;

        std::set<int> actionGroups;
        for (auto & action: actions)
        {
            actionGroups.insert(action.id);
        }

        for (int actionId: actionGroups)
        {
            for (auto & action: actions)
            {
                if (action.id == actionId)
                {
                    LOG_DEBUG << "  " << action.id << "/" << action.parameter;

                    auto * actionButton = actionsPanel->createWidget<MyGUI::Button>("Button", MyGUI::IntCoord(buttonLeft, buttonTop, 50, 50), MyGUI::Align::Default);
                    m_actionButtons.push_back(actionButton);
                    buttonLeft += 50;

                    actionButton->setCaption(action.name);
                    actionButton->setUserData(action);
                    actionButton->eventMouseButtonClick += MyGUI::newDelegate(this, &ActionsView::actionClicked);
                    actionButton->eventMouseSetFocus += MyGUI::newDelegate(this, &ActionsView::actionMouseSetFocus);
                    actionButton->eventMouseLostFocus += MyGUI::newDelegate(this, &ActionsView::actionMouseLostFocus);
                }
            }
            buttonTop += 50;
            buttonLeft = 0;
        }
    }
}


