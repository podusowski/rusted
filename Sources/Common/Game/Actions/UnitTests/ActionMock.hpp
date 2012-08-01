#pragma once

#include <gmock/gmock.h>

#include "Common/Game/Actions/IAction.hpp"

namespace Common
{
namespace Game
{
namespace Actions
{

class ActionMock : public Common::Game::Actions::IAction
{
public:
    MOCK_METHOD0(execute, void());
};

}
}
}
