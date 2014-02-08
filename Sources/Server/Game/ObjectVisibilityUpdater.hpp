#pragma once

#include "Cake/DependencyInjection/Inject.hpp"

#include "Game/IPlayerContainer.hpp"
#include "Common/Game/Universe.hpp"

namespace Server
{
namespace Game
{

class ObjectVisibilityUpdater
{
public:
    void sendVisibleObjects(const Common::Game::Position &);

private:
    Cake::DependencyInjection::Inject<Common::Game::Universe> m_universe;
    Cake::DependencyInjection::Inject<Game::IPlayerContainer> m_playerContainer;
};

}
}
