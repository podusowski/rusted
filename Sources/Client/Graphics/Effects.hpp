#pragma once

#include "Common/Game/Position.hpp"
#include "Graphics/IGraphics.hpp"

namespace Client
{
namespace Graphics
{

class Effects 
{
public:
    Effects(IGraphics &);
    void emitMovingBillboardEffect(Common::Game::Position start, Common::Game::Position end, unsigned speed);

private:
    IGraphics & m_graphics;
};

}
}

