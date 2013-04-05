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

PlayerState Player::getState()
{
    return m_state;
}

void Player::setState(PlayerState state)
{
    m_state = state;
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

unsigned Player::getHelium()
{
    return m_helium;
}

void Player::setHelium(unsigned helium)
{
    m_helium = helium;
}

unsigned Player::getCarbon()
{
    return m_carbon;
}

void Player::setCarbon(unsigned carbon)
{
    m_carbon = carbon;
}

std::string Player::getName()
{
    return m_name;
}

void Player::setName(const std::string & name)
{
    m_name = name;
}

