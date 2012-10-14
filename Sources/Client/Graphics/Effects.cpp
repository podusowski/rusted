#include "Cake/Diagnostics/Logger.hpp"

#include "MovingMeshEffect.hpp"
#include "Effects.hpp"

using namespace Client::Graphics;

Effects::Effects(IGraphics & graphics) : m_graphics(graphics)
{
}

void Effects::frameStarted()
{
    for (auto & effect : m_effects)
    {
        effect->frameStarted();
    }
}

void Effects::emitMovingMeshEffect(Common::Game::Position start, Common::Game::Position end, unsigned speed)
{
    boost::shared_ptr<IEffect> effect(new MovingMeshEffect(m_graphics, start, end));
    addEffect(effect);
}

void Effects::addEffect(boost::shared_ptr<IEffect> effect)
{
    LOG_DEBUG << "Creating effect: " << TYPENAME(*effect);

    m_effects.push_back(effect);
}
