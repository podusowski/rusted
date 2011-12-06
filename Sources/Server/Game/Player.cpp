#include "Game/Player.hpp"

using namespace Server::Game;

Player::Player(int id) : m_id(id)
{
}

int Player::getId()
{
    return m_id;
}
