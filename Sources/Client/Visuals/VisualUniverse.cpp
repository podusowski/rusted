#include <stdexcept>

#include "VisualUniverse.hpp"

using namespace Client::Visuals;

VisualUniverse::VisualUniverse(
    Client::Graphics::IGraphics & graphics, 
    Client::Gui::Gui & gui,
    Client::Input::IInput & input,
    Client::Services::ObjectService & objectService)
    :
    m_graphics(graphics),
    m_gui(gui),
    m_input(input),
    m_objectService(objectService)
{
}

std::shared_ptr<VisualObject> VisualUniverse::add(Common::Game::Object::ObjectBase & object)
{
    std::shared_ptr<VisualObject> obj(new VisualObject(m_graphics, m_gui, m_input, object, m_objectService));
    m_objects.insert(std::make_pair(&object, obj));
    return obj;
}

std::shared_ptr<VisualObject> VisualUniverse::find(Common::Game::Object::ObjectBase & object)
{
    auto it = m_objects.find(&object);
    if (it != m_objects.end())
    {
        return it->second;
    }

    throw std::out_of_range("");
}

void VisualUniverse::update()
{
    for (auto o: m_objects)
    {
        o.second->update();
    }
}


