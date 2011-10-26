#pragma once

#include "Engine/Engine.hpp"
#include "Network/Connection.hpp"
#include "Common/Configuration/Configuration.hpp"

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
    Common::Configuration::Configuration m_cfg;
    Engine m_engine;
};

}
}

