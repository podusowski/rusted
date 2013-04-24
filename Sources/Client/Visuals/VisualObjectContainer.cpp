#include <stdexcept>

#include "VisualObjectContainer.hpp"

using namespace Client::Visuals;

VisualObjectContainer::VisualObjectContainer(
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

std::shared_ptr<VisualObject> VisualObjectContainer::add(Common::Game::Object::ObjectBase & object)
{
    std::shared_ptr<VisualObject> obj(new VisualObject(m_graphics, m_gui, m_input, object, m_objectService));
    m_objects.insert(std::make_pair(&object, obj));
    return obj;
}

std::shared_ptr<VisualObject> VisualObjectContainer::find(Common::Game::Object::ObjectBase & object)
{
    auto it = m_objects.find(&object);
    if (it != m_objects.end())
    {
        return it->second;
    }

    throw std::out_of_range("");
}

void VisualObjectContainer::update()
{
    for (auto o: m_objects)
    {
        o.second->update();
    }
}


