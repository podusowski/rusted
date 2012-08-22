#pragma once

#include "Common/Game/TimeValue.hpp"

namespace Server 
{
namespace Game
{
namespace Actions
{

class IAction
{
public:
    virtual Common::Game::TimeValue start() = 0;
    virtual void finish() = 0;

    virtual ~IAction() {}
};

}
}
}
