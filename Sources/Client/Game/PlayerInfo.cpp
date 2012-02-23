#include "Cake/Diagnostics/Logger.hpp"
#include "Client/Game/PlayerInfo.hpp"

using namespace Client::Game;

void PlayerInfo::setId(int id)
{
    LOG_DEBUG << "New player id: " << id;

    m_id = id;
}

int PlayerInfo::getId()
{
    if (!m_id)
    {
        LOG_ERR << "Player id was never set";
        return 0;
    }
    else
    {
        return *m_id; 
    }
}

