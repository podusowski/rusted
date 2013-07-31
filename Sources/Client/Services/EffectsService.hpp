#pragma once

#include "Forwards.hpp"

#include "Services/AbstractService.hpp"
#include "Visuals/Effects.hpp"

namespace Client
{
namespace Services
{

class EffectsService : public AbstractService<EffectsService>
{
public:
    EffectsService(Visuals::VisualUniverse &, Visuals::Effects &);

    void handle(const Common::Messages::EmitMovingMeshEffect &);
    void handle(const Common::Messages::EmitExplosionEffect &);

    void handle(const Common::Messages::AbstractMessage &) {}

private:
    Visuals::VisualUniverse & m_visualUniverse;
    Visuals::Effects & m_effects;
};

}
}
