#include "Engine/Application.hpp"

using namespace Client::Engine;

Application::Application(int argc, const char * argv[]) :
    m_cfg(argc, argv),
    m_engine(m_cfg)
{
}

void Application::start()
{
    m_engine.start();
}

