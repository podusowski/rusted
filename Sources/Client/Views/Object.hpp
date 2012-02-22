#pragma once

#include "Client/Graphics/IGraphics.hpp"
#include "Common/Game/Object/ObjectBase.hpp"

namespace Client
{
namespace Views
{

class Object
{
public:
    Object(Client::Graphics::IGraphics &, Common::Game::Object::ObjectBase &);
    void update();

private:
    Client::Graphics::IGraphics & m_graphics;
    Common::Game::Object::ObjectBase & m_object;
    Ogre::SceneNode * m_node;
};

}
}
