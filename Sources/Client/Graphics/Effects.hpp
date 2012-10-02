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
    void emitSingleLinearEffect(Common::Game::Position start, Common::Game::Position end, unsigned speed);

private:
    IGraphics & m_graphics;
};

}
}

