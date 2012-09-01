#pragma once

#include "Common/Game/Player.hpp"
#include "Client/Graphics/IGraphics.hpp"

namespace Client
{
namespace Views
{

class Camera
{
public:
    Camera(Graphics::IGraphics &, Common::Game::Player &);
    ~Camera();

    void update();

private:
    Graphics::IGraphics & m_graphics;
    Common::Game::Player & m_player;
};

}
}
