#pragma once

#include <stack>

#include "Gui/Gui.hpp"
#include "States/IState.hpp"
#include "States/IStateManagerStack.hpp"

namespace Client
{
namespace States
{

class StateManager : public IStateManagerStack
{
public:
    StateManager();
    void frameStarted();
    /* IStateManagerStack */ void pushState(IState &);
    /* IStateManagerStack */ IState & popState();

private:
    IState * m_currentState;
    std::stack<IState *> m_stateStack;
};

}
}
