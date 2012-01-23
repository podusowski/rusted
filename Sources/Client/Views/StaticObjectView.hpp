#pragma once

#include "Common/Game/Object/StaticObject.hpp"
#include "Client/Views/IView.hpp"
#include "Client/Services/StaticObjectService.hpp"
#include "Client/Graphics/IGraphics.hpp"

namespace Client
{
namespace Views
{

class StaticObjectView : public IView
{
public:
    StaticObjectView(Services::StaticObjectService &,
                     Graphics::IGraphics &,
                     Common::Game::Universe &);

    void activate();
    void deactivate();
    void frameStarted();

    void objectAdded(Common::Game::Object::ObjectBase &);

private:
    Services::StaticObjectService & m_staticObjectService;
    Graphics::IGraphics & m_graphics;
    Common::Game::Universe & m_universe;
};

}
}
