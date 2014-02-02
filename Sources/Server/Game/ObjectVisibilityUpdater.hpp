#pragma once

#include "Common/Game/Universe.hpp"

namespace Server
{
namespace Game
{

class ObjectVisibilityUpdater
{
public:
    void sendVisibleObjects(const Common::Game::Position &);
};

}
}
