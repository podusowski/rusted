#ifndef PLAYER_HPP
#define PLAYER_HPP

namespace Server
{
namespace Game
{

class Player
{
public:
    Player(int id);
    int getId();

private:
    int m_id;
};

}
}

#endif

