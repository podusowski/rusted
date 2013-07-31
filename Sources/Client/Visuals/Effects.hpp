#pragma once

#include <boost/shared_ptr.hpp>

#include "VisualObject.hpp"
#include "Common/Game/Position.hpp"
#include "Graphics/IGraphics.hpp"
#include "IEffect.hpp"

namespace Client
{
namespace Visuals
{

class Effects
{
public:
    Effects(Graphics::IGraphics &);
    void frameStarted();
    void emitMovingMeshEffect(Common::Game::Position start, Common::Game::Position end, unsigned speed);
    void emitExplosion(VisualObject & object, Common::Game::Position direction);

private:
    void addEffect(boost::shared_ptr<IEffect>);
    void gc();

    Graphics::IGraphics & m_graphics;
    std::vector<boost::shared_ptr<IEffect> > m_effects;
};

}
}

