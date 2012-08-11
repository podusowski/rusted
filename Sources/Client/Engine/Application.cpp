#include "Engine/Application.hpp"

using namespace Client::Engine;

Application::Application(int argc, const char * argv[])
{
}

void Application::start()
{
    m_engine.start();
}

