#include "Gather.hpp"

using namespace Server::Game::Actions;

Gather::Gather(Common::Game::Object::ObjectBase & object) : 
    m_asteroid(dynamic_cast<Common::Game::Object::Asteroid&>(object))
{
}

Common::Game::TimeValue Gather::start()
{
    return Common::Game::TimeValue(0, 0);
}

void Gather::finish()
{
}

