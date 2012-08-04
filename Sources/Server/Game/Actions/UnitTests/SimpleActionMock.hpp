#pragma once

#include <gmock/gmock.h>

#include "Common/Game/Actions/ISimpleAction.hpp"

namespace Common
{
namespace Game
{
namespace Actions
{

class SimpleActionMock : public Common::Game::Actions::ISimpleAction
{
public:
    MOCK_METHOD0(execute, void());
};

}
}
}
