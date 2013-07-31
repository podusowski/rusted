#pragma once

#include "Services/AbstractService.hpp"
#include "Visuals/Effects.hpp"

namespace Client
{
namespace Services
{

class EffectsService : public AbstractService<EffectsService>
{
public:
    EffectsService(Visuals::Effects &);

    void handle(const Common::Messages::EmitMovingMeshEffect &);
    void handle(const Common::Messages::EmitExplosionEffect &);

    void handle(const Common::Messages::AbstractMessage &) {}

private:
    Visuals::Effects & m_effects;
};

}
}
