#include "EffectsService.hpp"

using namespace Client::Services;

EffectsService::EffectsService(Effects::Effects & effects) : m_effects(effects)
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
    Common::Game::Position position(
        emitExplosion.x, emitExplosion.y, emitExplosion.z
    );

    m_effects.emitExplosion(position);
}

