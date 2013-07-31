#include "Cake/Diagnostics/Logger.hpp"

#include "MovingMeshEffect.hpp"
#include "Explosion.hpp"
#include "Effects.hpp"

using namespace Client::Visuals;

Effects::Effects(Client::Graphics::IGraphics & graphics) : m_graphics(graphics)
{
}

void Effects::frameStarted()
{
    for (auto & effect : m_effects)
    {
        effect->frameStarted();
    }

    gc();
}

void Effects::emitMovingMeshEffect(Common::Game::Position start, Common::Game::Position end, unsigned speed)
{
    boost::shared_ptr<IEffect> effect(new MovingMeshEffect(m_graphics, start, end, speed));
    addEffect(effect);
}

void Effects::emitExplosion(VisualObject & object, Common::Game::Position direction)
{
    boost::shared_ptr<IEffect> effect(new Explosion(m_graphics, object, direction));
    addEffect(effect);
}

void Effects::addEffect(boost::shared_ptr<IEffect> effect)
{
    LOG_DEBUG << "Creating effect: " << TYPENAME(*effect);

    m_effects.push_back(effect);
}

void Effects::gc()
{
    auto it = m_effects.begin();
    while (it != m_effects.end())
    {
        if ((*it)->isAlive())
        {
            it++;
        }
        else
        {
            LOG_DEBUG << "Collecting dead effect: " << TYPENAME(**it);
            it = m_effects.erase(it);
        }
    }
}

