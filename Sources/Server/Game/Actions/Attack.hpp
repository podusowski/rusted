#pragma once

#include "Server/Game/Actions/IAction.hpp"
#include "Server/Game/IPlayerContainer.hpp"
#include "Common/Game/Object/Ship.hpp"
#include "Server/Services/Utils.hpp"
#include "ActionParameters.hpp"

namespace Server
{
namespace Game
{
namespace Actions
{

class Attack : public IAction
{
public:
    Attack(Common::Game::Universe & universe, IPlayerContainer &, const ActionParameters & actionParameters);
    Common::Game::TimeValue start();
    Common::Game::TimeValue finish();

private:
    void sendShipInfoToClients();
    void sendMovingMeshEffect();
    void sendExplosionEffect();

    Common::Game::Universe & m_universe;
    IPlayerContainer & m_playerContainer;
    ActionParameters m_actionParameters;
    Server::Services::Utils m_servicesUtils;
    int m_speed;
};

}
}
}

