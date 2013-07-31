#include "Visuals/VisualUniverse.hpp"
#include "EffectsService.hpp"

using namespace Client::Services;

EffectsService::EffectsService(Visuals::VisualUniverse & visualUniverse, Visuals::Effects & effects) :
    m_effects(effects),
    m_visualUniverse(visualUniverse)
{
}

void EffectsService::handle(const Common::Messages::EmitMovingMeshEffect & emitMovingMeshEffect)
{
    Common::Game::Position from(
        emitMovingMeshEffect.fromX, emitMovingMeshEffect.fromY, emitMovingMeshEffect.fromZ
    );

    Common::Game::Position to(
        emitMovingMeshEffect.toX, emitMovingMeshEffect.toY, emitMovingMeshEffect.toZ
    );

    m_effects.emitMovingMeshEffect(from, to, emitMovingMeshEffect.speed);
}

void EffectsService::handle(const Common::Messages::EmitExplosionEffect & emitExplosion)
{
    auto object = m_visualUniverse.find(emitExplosion.objectId);

    Common::Game::Position direction(
        emitExplosion.directionX,
        emitExplosion.directionY,
        emitExplosion.directionZ
    );

    m_effects.emitExplosion(*object, direction);
}

