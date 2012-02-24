#pragma once

#include "Common/Game/Universe.hpp"
#include "Client/States/IState.hpp"
#include "Client/Gui/Gui.hpp"
#include "Client/Network/Connection.hpp"
#include "Client/States/IStateManagerStack.hpp"
#include "Client/Services/EntityService.hpp"
#include "Client/States/PilotState.hpp"

namespace Client
{
namespace States
{

class MyListItem : public CEGUI::ListboxTextItem
{
public:
    MyListItem (const std::string & text, CEGUI::uint item_id = 0) :
        ListboxTextItem(text, item_id)
    {
        setSelectionBrushImage("TaharezLook", "MultiListSelectionBrush");
    }
};

class EntitySelectState : public IState
{
public:
    EntitySelectState(IStateManagerStack &,
                      Gui::Gui &, 
                      Network::Connection &,
                      Services::EntityService &,
                      Services::ObjectService &,
                      States::PilotState &,
                      Common::Game::Universe &,
                      Client::Game::PlayerInfo &);
    void activate();
    void deactivate();
    void frameStarted();

private:
    void myEntitiesFetched();
    bool flyButtonClicked(const CEGUI::EventArgs &);

    States::IStateManagerStack & m_stateManagerStack;
    Gui::Gui & m_gui;
    Services::EntityService & m_entityService;
    Services::ObjectService & m_objectService;
    CEGUI::Window * m_layout;
    States::PilotState & m_pilotState;
    Common::Game::Universe & m_universe;
    Client::Game::PlayerInfo & m_playerInfo;
};

}
}

