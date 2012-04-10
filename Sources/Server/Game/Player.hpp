#pragma once

namespace Server
{
namespace Game
{

enum PlayerState
{
    PLAYER_STATE_NEW,
    PLAYER_STATE_AUTHORIZED
};

class Player
{
public:
    Player();
    void setId(int);
    int getId();

    void setState(PlayerState);
    PlayerState getState();

private:
    int m_id;
    PlayerState m_state;
};

}
}

