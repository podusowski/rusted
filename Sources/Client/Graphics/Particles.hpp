#pragma once

#include "Common/Game/Position.hpp"
#include "Graphics/IGraphics.hpp"

namespace Client
{
namespace Graphics
{

class Particles
{
public:
    Particles(IGraphics &);
    void emitSingleLinearParticle(Common::Game::Position start, Common::Game::Position end, unsigned speed);

private:
    IGraphics & m_graphics;
};

}
}

