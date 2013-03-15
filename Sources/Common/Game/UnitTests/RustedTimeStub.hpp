#pragma once

#include <gmock/gmock.h>

#include "Common/Game/IRustedTime.hpp"

class RustedTimeStub : public Common::Game::IRustedTime
{
public:
    MOCK_METHOD0(getSeconds, unsigned());
    MOCK_METHOD0(getCurrentTime, Common::Game::TimeValue());
    MOCK_METHOD0(getAbsoluteTime, Common::Game::TimeValue());
    MOCK_METHOD1(setReferenceTime, void(Common::Game::TimeValue));
    MOCK_METHOD2(createTimer, void(Common::Game::TimeValue, boost::function<void()>));
};

