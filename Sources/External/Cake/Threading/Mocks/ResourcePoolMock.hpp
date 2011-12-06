#pragma once

#include <gmock/gmock.h>
#include "Common/Threading/IResourcePool.hpp"

namespace Common
{
namespace Threading 
{

class ResourcePoolMock : public IResourcePool
{
public:
    MOCK_METHOD0(aquire, void());
    MOCK_METHOD0(release, void());
};

}
}
