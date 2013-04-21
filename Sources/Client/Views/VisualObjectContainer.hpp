#pragma once

#include <map>

#include "Common/Game/Object/ObjectBase.hpp"
#include "Graphics/IGraphics.hpp"
#include "Gui/Gui.hpp"
#include "Input/IInput.hpp"
#include "Services/ObjectService.hpp"
#include "VisualObject.hpp"

namespace Client
{
namespace Views
{

class VisualObjectContainer
{
public:
    VisualObjectContainer(Client::Graphics::IGraphics &, Client::Gui::Gui &, Client::Input::IInput &, Client::Services::ObjectService &);
    std::shared_ptr<VisualObject> add(Common::Game::Object::ObjectBase &);
    std::shared_ptr<VisualObject> find(Common::Game::Object::ObjectBase &);
    void update();
    
private:
    Client::Graphics::IGraphics & m_graphics;
    Client::Gui::Gui & m_gui;
    Client::Input::IInput & m_input;
    Client::Services::ObjectService & m_objectService;
    std::map<Common::Game::Object::ObjectBase*, std::shared_ptr<VisualObject>> m_objects;
};

}
}

