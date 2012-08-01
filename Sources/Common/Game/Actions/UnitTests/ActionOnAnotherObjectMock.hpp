#pragma once

#include <gmock/gmock.h>

#include "Common/Game/Actions/IActionOnAnotherObject.hpp"

namespace Common
{
namespace Game
{
namespace Actions
{

class ActionOnAnotherObjectMock : public Common::Game::Actions::IActionOnAnotherObject
{
public:
    MOCK_METHOD1(execute, void(Common::Game::Object::ObjectBase &));
};

}
}
}
