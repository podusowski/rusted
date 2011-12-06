#include "Common/Logger/Logger.hpp"
#include "Client/States/StateManager.hpp"

using namespace Client::States;

StateManager::StateManager() : m_currentState(0) {}

void StateManager::frameStarted()
{
    if (!m_currentState)
    {
        LOG_ERR << "No state to execute, application is pointless\n";
        return;
    }

    m_currentState->frameStarted();
}

void StateManager::pushState(IState & state)
{
    if (m_currentState)
    {
        LOG_INFO << "Deactivating " << TYPENAME(*m_currentState) << "\n";
        m_currentState->deactivate();
    }
    LOG_INFO << "Pushing and activating " << TYPENAME(state) << "\n";
    m_stateStack.push(&state);
    m_currentState = m_stateStack.top();
    m_currentState->activate();
}

IState & StateManager::popState()
{
    if (!m_stateStack.empty())
    {
        IState * state = m_stateStack.top();
        m_stateStack.pop();
        if (!m_stateStack.empty())
        {
            m_currentState = m_stateStack.top();
        }
        else
        {
            LOG_WARN << "The last state was poped, we should close the application now, shouldn't we?\n";
        }
        return *state;
    }
    else
    {
        throw std::out_of_range("no states to pop");
    }
}

