#pragma once

#include <functional>

#include "Client/Graphics/IGraphics.hpp"
#include "Client/Input/IInput.hpp"
#include "Common/Game/Object/ObjectBase.hpp"

namespace Client
{
namespace Views
{

class Object
{
public:
    Object(Client::Graphics::IGraphics &, Client::Input::IInput &, Common::Game::Object::ObjectBase &);

    void setRightClickCallback(std::function<void()>);
    void setSelected(bool);

    void update();

private:
    void rightClickedCallback();

    Client::Graphics::IGraphics & m_graphics;
    Common::Game::Object::ObjectBase & m_object;
    Ogre::Entity * m_entity;
    Ogre::SceneNode * m_node;
    std::function<void()> m_rightClickCallback;
};

}
}
