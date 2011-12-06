#include "Common/Logger/Logger.hpp"
#include "Client/Game/PlayerInfo.hpp"

using namespace Client::Game;

void PlayerInfo::setId(int id)
{
    m_id = id;
}

int PlayerInfo::getId()
{
    if (!m_id)
    {
        LOG_ERR << "Player id was never set\n";
        return 0;
    }
    else
    {
        return *m_id; 
    }
}

