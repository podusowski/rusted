#include <boost/bind.hpp>

#include "ActionBuilder.hpp"
#include "Attack.hpp"
#include "BuildShip.hpp"

using namespace Server::Game::Actions;

ActionBuilder::ActionBuilder(Common::Game::Universe & universe, Server::Game::IPlayerContainer & playerContainer) :
    m_universe(universe),
    m_playerContainer(playerContainer)
{
}

boost::shared_ptr<IAction> ActionBuilder::build(
        Server::Network::IConnection & connection,
        Server::Game::IPlayer & player,
        unsigned id)
{
    boost::shared_ptr<IAction> ret;

    auto & focusedObject = player.getFocusedObject();
    auto & focusedShip = dynamic_cast<Common::Game::Object::Ship&>(focusedObject);

    Common::Game::Object::ObjectBase * selectedObject = nullptr;

    // FIXME: you know what - but not now, when we're going to develop object deletion, it
    //        will most likely be weak_ptr anyway
    try
    {
        selectedObject = &(player.getSelectedObject());
    }
    catch (...)
    {
    }

    aquireGlobalCooldown(player.getId());

    switch (id)
    {
        case 1:
        {
            assert(selectedObject);
            ret = boost::shared_ptr<IAction>(new Attack(m_playerContainer, focusedShip, *selectedObject));
            break;
        }

        case 2:
        {
            auto & player = m_playerContainer.getBy(connection);
            ret = boost::shared_ptr<IAction>(new BuildShip(m_universe, player, m_playerContainer));
            break;
        }

        default:
            throw std::runtime_error("unknown action");
    }

    if (ret)
    {
        LOG_DEBUG << "Constructed action: " << id << "/" << CAKE_DEPENDENCY_INJECTION_TYPENAME(*ret);
    }

    return ret;
}

void ActionBuilder::aquireGlobalCooldown(unsigned playerId)
{
    auto ret = m_playerGlobalCooldowns.insert(playerId);
    if (ret.second)
    {
        LOG_DEBUG << "Global cooldown activated on player: " << playerId;
        m_time->createTimer(Common::Game::TimeValue(1, 0), boost::bind(&ActionBuilder::globalCooldownExpired, this, playerId));
    }
    else
    {
        LOG_WARN << "Global cooldown is already active on player: " << playerId << ", this might suggest hack attempt";
        throw std::runtime_error("global cooldown is active");
    }
}

void ActionBuilder::globalCooldownExpired(unsigned playerId)
{
    LOG_DEBUG << "Global cooldown expired for player: " << playerId;
    size_t elementsErased = m_playerGlobalCooldowns.erase(playerId);
    if (elementsErased == 0)
    {
        throw std::runtime_error("expired nonexisting global cooldown");
    }
}

