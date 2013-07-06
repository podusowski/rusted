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
        unsigned _actionId,
        unsigned _actionParameter,
        Common::Game::Object::ObjectBase::StrictId _focusedObjectId,
        Common::Game::Object::ObjectBase::Id _selectedObjectId
    ) :
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

