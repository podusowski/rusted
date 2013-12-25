#pragma once

#include "Common/Game/Object/ObjectBase.hpp"

namespace Server
{
namespace Game
{
namespace Actions
{

struct ActionParameters
{
    ActionParameters(
        Common::Game::IPlayer::Id _playerId,
        unsigned _actionId,
        unsigned _actionParameter,
        Common::Game::Object::ObjectBase::StrictId _focusedObjectId,
        Common::Game::Object::ObjectBase::Id _selectedObjectId
    ) :
        playerId(_playerId),
        actionId(_actionId),
        actionParameter(_actionParameter),
        focusedObjectId(_focusedObjectId),
        selectedObjectId(_selectedObjectId)
    {
    }

    bool operator == (const ActionParameters & other) const
    {
        return
            actionId == other.actionId &&
            actionParameter == other.actionParameter &&
            focusedObjectId == other.focusedObjectId &&
            selectedObjectId == other.selectedObjectId;
    }

    Common::Game::IPlayer::Id playerId;
    unsigned actionId;
    unsigned actionParameter;
    Common::Game::Object::ObjectBase::StrictId focusedObjectId;
    Common::Game::Object::ObjectBase::Id selectedObjectId;
};

}
}
}

inline std::ostream & operator << (std::ostream & os, const Server::Game::Actions::ActionParameters & actionParameters)
{
    return os << "action:" << actionParameters.actionId << "/" << actionParameters.actionParameter
              << ", focusedObject:" << actionParameters.focusedObjectId
              << ", selectedObject:" << actionParameters.selectedObjectId;
}

