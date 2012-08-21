#pragma once

#include <gmock/gmock.h>

namespace Server
{
namespace Game
{
namespace Actions
{

class ActionMock : public IAction
{
public:
    MOCK_METHOD0(execute, void());
};

}
}
}

