#include "Attack.hpp"

using namespace Server::Game::Actions;

void Attack::execute(Common::Game::Object::ObjectBase & object)
{
    unsigned integrity = object.getIntegrity();
    object.setIntegrity(integrity - 10);
}

