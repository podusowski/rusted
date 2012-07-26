#include <stdexcept>

#include "Game/Player.hpp"

using namespace Server::Game;

Player::Player() : 
    m_selectedObject(nullptr),
    m_focusedObject(nullptr)
{
}

void Player::setId(int id)
{
    m_id = id;
}

int Player::getId()
{
    return m_id;
}

PlayerState Player::getState()
{
    return m_state;
}

void Player::setState(PlayerState state)
{
    m_state = state;
}

void Player::focusObject(Common::Game::Object::ObjectBase & object)
{
    m_focusedObject = &object;
}

Common::Game::Object::ObjectBase & Player::getFocusedObject()
{
    if (!m_focusedObject)
        throw std::runtime_error("player has no object focused");

    return *m_focusedObject;
}

void Player::selectObject(Common::Game::Object::ObjectBase & object)
{
    m_selectedObject = &object;
}

Common::Game::Object::ObjectBase & Player::getSelectedObject()
{
    if (!m_selectedObject)
        throw std::runtime_error("player has no object selected");

    return *m_selectedObject;
}

