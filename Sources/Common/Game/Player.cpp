#include <stdexcept>

#include "Player.hpp"

using namespace Common::Game;

Player::Player() : m_focusedObject(nullptr)
{
}

Player::~Player()
{
}

void Player::setId(int id)
{
    m_id = id;
}

int Player::getId()
{
    if (!m_id)
    {
        throw std::runtime_error("player id has never been set");
    }
    return *m_id;
}

void Player::focusObject(Common::Game::Object::ObjectBase & object)
{
    m_focusedObject = &object;
}

Common::Game::Object::ObjectBase & Player::getFocusedObject()
{
    if (!m_focusedObject)
    {
        throw std::runtime_error("no object focused");
    }
    return *m_focusedObject;
}

