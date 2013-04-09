#pragma once

#include "Services/AbstractService.hpp"
#include "Effects/Effects.hpp"

namespace Client
{
namespace Services
{

class EffectsService : public AbstractService<EffectsService>
{
public:
    EffectsService(Effects::Effects &);

    void handle(const Common::Messages::EmitMovingMeshEffect &);
    void handle(const Common::Messages::AbstractMessage &) {}

private:
    Effects::Effects & m_effects;
};

}
}
