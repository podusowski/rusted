#pragma once

#include "Common/Game/Object/ObjectBase.hpp"
#include "Client/Graphics/IGraphics.hpp"

namespace Client
{
namespace Views
{

class Camera
{
public:
    Camera(Graphics::IGraphics &);
    ~Camera();

    void update();

private:
    Graphics::IGraphics & m_graphics;
};

}
}
