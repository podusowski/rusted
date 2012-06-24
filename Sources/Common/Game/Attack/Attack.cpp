#include "Attack.hpp"

using namespace Common::Game::Attack;

void Attack::attack(Common::Game::Object::ObjectBase & attackedObject)
{
    auto integrity = attackedObject.getIntegrity();
    attackedObject.setIntegrity(integrity - 10);
}
