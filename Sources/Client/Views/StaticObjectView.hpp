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
                     Graphics::IGraphics &);

    void activate();
    void deactivate();
    void frameStarted();

    void staticObjectAdded(Common::Game::Object::StaticObject &);

private:
    Services::StaticObjectService & m_staticObjectService;
    Graphics::IGraphics & m_graphics;
};

}
}
