#pragma once

#include <gmock/gmock.h>

#include "Common/Game/IPlayer.hpp"

namespace Common
{
namespace Game
{

class PlayerMock : public IPlayer
{
public:
    MOCK_METHOD1(setId, void(int));
    MOCK_METHOD0(getId, int());
    MOCK_METHOD1(setState, void(PlayerState));
    MOCK_METHOD0(getState, PlayerState());
    MOCK_METHOD1(selectObject, void(Common::Game::Object::ObjectBase &));
    MOCK_METHOD0(getSelectedObject, Common::Game::Object::ObjectBase & ());
    MOCK_METHOD0(getSelectedObjectId, Common::Game::Object::ObjectBase::Id());
    MOCK_METHOD1(focusObject, void(Common::Game::Object::ObjectBase &));
    MOCK_METHOD0(getFocusedObject, Common::Game::Object::ObjectBase & ());
    MOCK_METHOD0(getFocusedObjectId, Common::Game::Object::ObjectBase::Id());
};

}  // namespace Game
}  // namespace Server

