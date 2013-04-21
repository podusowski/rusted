#include "VisualObjectContainer.hpp"

using namespace Client::Views;

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

void VisualObjectContainer::update()
{
    for (auto o: m_objects)
    {
        o.second->update();
    }
}


