#pragma once

#include <functional>
#include <boost/utility.hpp>

#include "Cake/Serialization/Tms.hpp"

#include "Common/Game/Object/ObjectBase.hpp"
#include "Client/Graphics/IGraphics.hpp"
#include "Gui/Gui.hpp"
#include "Client/Input/IInput.hpp"
#include "Client/Services/ObjectService.hpp"

namespace Client
{
namespace Visuals
{

class VisualObject : boost::noncopyable
{
public:
    static const int StringType_All = 0xFF;
    static const int StringType_Class = 1 << 0;
    static const int StringType_Integrity = 1 << 1;
    static const int StringType_Pilot = 1 << 2;
    static const int StringType_CargoHold = 1 << 3;
    static const int StringType_Position = 1 << 4;
    static const int StringType_Speed = 1 << 5;

    VisualObject(
        Client::Graphics::IGraphics &,
        Gui::Gui &,
        Client::Input::IInput &,
        Common::Game::Object::ObjectBase &,
        Client::Services::ObjectService &);

    void setRightClickCallback(std::function<void()>);
    void setSelected(bool);
    Common::Game::Object::ObjectBase & getGameObject();
    std::string getString(int type = StringType_All);
    Ogre::SceneNode & getOgreSceneNode();
    Ogre::Entity & getOgreEntity();

    void update();

private:
    void entityClickedCallback();
    void entityMouseMoved();
    void entityMouseLeaved();
    void setEngineThrustEnabled(bool);
    void createLabel();
    void updateLabelText();
    void updateLabel();
    void createEngineThrustEffect();
    void ownerNameFetched(unsigned id, const std::string & name);
    void updateWreckedState();

    Client::Graphics::IGraphics & m_graphics;
    Gui::Gui & m_gui;
    Common::Game::Object::ObjectBase & m_object;
    Client::Services::ObjectService & m_objectService;
    Ogre::Entity * m_entity;
    Ogre::SceneNode * m_mainNode;
    Ogre::SceneNode * m_meshNode;
    std::function<void()> m_rightClickCallback;
    std::vector<Ogre::ParticleSystem *> m_engineThrustParticleSystems;
    MyGUI::TextBox * m_label;
    std::string m_ownerName;
    Cake::Serialization::Tms m_model;
    std::string m_naturalMaterial;
};

}
}
