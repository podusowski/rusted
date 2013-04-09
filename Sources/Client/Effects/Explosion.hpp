#pragma once

#include "Graphics/IGraphics.hpp"
#include "IEffect.hpp"

namespace Client
{
namespace Effects
{

class Explosion : public IEffect
{
public:
    Explosion(Graphics::IGraphics &, Common::Game::Position);
    void frameStarted(); 
    bool isAlive();

private:
    static int m_id;

    Graphics::IGraphics & m_graphics;
    Ogre::ParticleSystem * m_ps;
};

}
}

