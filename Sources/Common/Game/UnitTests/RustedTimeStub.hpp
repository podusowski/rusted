#pragma once

#include "Common/Game/IRustedTime.hpp"

class RustedTimeStub : public Common::Game::IRustedTime
{
public:
    RustedTimeStub() : m_seconds(0) {}
    unsigned getSeconds() { return m_seconds; }
    void setSeconds(unsigned seconds) { m_seconds = seconds; }
    void setReferenceTime(unsigned) {}

private:
    unsigned m_seconds;
};

