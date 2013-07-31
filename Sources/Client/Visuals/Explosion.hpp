#pragma once

#include "VisualObject.hpp"

#include "Graphics/IGraphics.hpp"
#include "IEffect.hpp"

namespace Client
{
namespace Visuals
{

class Explosion : public IEffect
{
public:
    Explosion(Graphics::IGraphics &, VisualObject & object, Common::Game::Position direction);
    void frameStarted(); 
    bool isAlive();

private:
    static int m_id;

    Graphics::IGraphics & m_graphics;
    VisualObject & m_object;
    Common::Game::Position m_direction;
    Ogre::ParticleSystem * m_ps;
};

}
}

