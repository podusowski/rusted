#pragma once

#include <functional>

#include "Cake/Serialization/Tms.hpp"

#include "Common/Game/Object/ObjectBase.hpp"
#include "Client/Graphics/IGraphics.hpp"
#include "Gui/Gui.hpp"
#include "Client/Input/IInput.hpp"
#include "Client/Services/ObjectService.hpp"

namespace Client
{
namespace Views
{

class VisualObject
{
public:
    enum StringType
    {
        StringType_All = 0xFF,
        StringType_Class = 1 << 0,
        StringType_Integrity = 1 << 1,
        StringType_Pilot = 1 << 2
    };

    VisualObject(
        Client::Graphics::IGraphics &,
        Gui::Gui &,
        Client::Input::IInput &,
        Common::Game::Object::ObjectBase &,
        Client::Services::ObjectService &);

    void setRightClickCallback(std::function<void()>);
    void setSelected(bool);
    Common::Game::Object::ObjectBase & getGameObject();
    std::string getString(StringType type = StringType_All);

    void update();

private:
    void entityClickedCallback();
    void entityMouseMoved();
    void entityMouseLeaved();
    void setEngineThrustEnabled(bool);
    void createLabel();
    void updateLabelText();
    void updateLabel();
    void ownerNameFetched(unsigned id, const std::string & name);

    Client::Graphics::IGraphics & m_graphics;
    Gui::Gui & m_gui;
    Common::Game::Object::ObjectBase & m_object;
    Client::Services::ObjectService & m_objectService;
    Ogre::Entity * m_entity;
    Ogre::SceneNode * m_node;
    std::function<void()> m_rightClickCallback;
    std::vector<Ogre::ParticleSystem *> m_engineThrustParticleSystems;
    MyGUI::TextBox * m_label;
    std::string m_ownerName;
    Cake::Serialization::Tms m_model;
};

}
}
