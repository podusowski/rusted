#include "Attack.hpp"

using namespace Common::Game::Attack;

void Attack::operator()(Common::Game::Object::ObjectBase & attackedObject)
{
    auto integrity = attackedObject.getIntegrity();
    attackedObject.setIntegrity(integrity - 10);
}
