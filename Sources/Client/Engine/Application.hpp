#pragma once

#include "Engine/Engine.hpp"
#include "Network/Connection.hpp"

namespace Client
{
namespace Engine
{

class Application
{
public:
    Application(int argc, const char * argv[]);
    void start();
private:
    Engine m_engine;
};

}
}

