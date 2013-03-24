#pragma once

#include <functional>

#include "Client/Graphics/IGraphics.hpp"
#include "Client/Input/IInput.hpp"
#include "Common/Game/Object/ObjectBase.hpp"

namespace Client
{
namespace Views
{

class VisualObject
{
public:
    VisualObject(
        Client::Graphics::IGraphics &,
        Client::Input::IInput &,
        Common::Game::Object::ObjectBase &);

    void setRightClickCallback(std::function<void()>);
    void setSelected(bool);
    Common::Game::Object::ObjectBase & getGameObject();

    void update();

private:
    void rightClickedCallback();
    void setEngineThrustEnabled(bool);

    Client::Graphics::IGraphics & m_graphics;
    Common::Game::Object::ObjectBase & m_object;
    Ogre::Entity * m_entity;
    Ogre::SceneNode * m_node;
    std::function<void()> m_rightClickCallback;
    std::vector<Ogre::ParticleSystem *> m_engineThrustParticleSystems;
};

}
}
