#include <stdexcept>

#include "Player.hpp"

using namespace Common::Game;

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

