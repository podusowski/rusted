#pragma once

#include <boost/shared_ptr.hpp>

#include "Common/Game/Position.hpp"
#include "Graphics/IGraphics.hpp"
#include "IEffect.hpp"

namespace Client
{
namespace Graphics
{

class Effects 
{
public:
    Effects(IGraphics &);
    void frameStarted();
    void emitMovingMeshEffect(Common::Game::Position start, Common::Game::Position end, unsigned speed);

private:
    void addEffect(boost::shared_ptr<IEffect>);
    void gc();

    IGraphics & m_graphics;
    std::vector<boost::shared_ptr<IEffect> > m_effects;
};

}
}

