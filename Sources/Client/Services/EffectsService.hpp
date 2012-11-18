#pragma once

#include "Services/AbstractService.hpp"
#include "Graphics/Effects.hpp"

namespace Client
{
namespace Services
{

class EffectsService : public AbstractService<EffectsService>
{
public:
    EffectsService(Graphics::Effects &);

    void handle(const Common::Messages::EmitMovingMeshEffect &);
    void handle(const Common::Messages::AbstractMessage &) {}

private:
    Graphics::Effects & m_effects;
};

}
}
