#pragma once

#include <gmock/gmock.h>

#include "Common/Game/IRustedTime.hpp"

class RustedTimeStub : public Common::Game::IRustedTime
{
public:
    MOCK_METHOD0(getSeconds, unsigned());
    MOCK_METHOD1(setReferenceTime, void(unsigned));
};

