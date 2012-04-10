#include "Game/Player.hpp"

using namespace Server::Game;

Player::Player()
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
