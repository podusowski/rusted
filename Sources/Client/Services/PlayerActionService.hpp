#pragma once

#include <boost/optional.hpp>

#include "Cake/DependencyInjection/Inject.hpp"

#include "Common/Game/IRustedTime.hpp"
#include "Common/Game/Universe.hpp"
#include "Common/Game/Object/Ship.hpp"
#include "Common/Game/Player.hpp"

#include "Client/Network/IConnectionListener.hpp"
#include "Client/Network/IConnection.hpp"
#include "Client/Services/AbstractService.hpp"

namespace Client
{
namespace Services
{

class PlayerActionService : public AbstractService<PlayerActionService> 
{
public:
    typedef boost::function<void(std::vector<boost::tuple<int, std::string> >)> AvailableActionsFetchedCallback;
    typedef boost::function<void()> GlobalCooldownExpiredCallback;

    PlayerActionService(Network::IConnection &, Common::Game::Player &, Common::Game::Universe &);

    void focusObject(Common::Game::Object::ObjectBase &);
    void setFocusedObjectCourse(Common::Game::Position);
    void selectObject(Common::Game::Object::ObjectBase &);
    void fetchAvailableActions(AvailableActionsFetchedCallback);
    void executeAction(unsigned actionId, GlobalCooldownExpiredCallback);

    void handle(const Common::Messages::AvailableActions &);
    void handle(const Common::Messages::GlobalCooldownExpired &);

    void handle(const Common::Messages::AbstractMessage &) {}

private:
    Common::Game::Player & m_player;
    Client::Network::IConnection & m_connection;
    Cake::DependencyInjection::Inject<Common::Game::IRustedTime> m_time;
    Common::Game::Universe & m_universe;

    boost::optional<Common::Game::Object::ObjectBase *> m_selectedObject;

    AvailableActionsFetchedCallback m_availableActionsFetchedCallback;
    GlobalCooldownExpiredCallback m_globalCooldownExpiredCallback;
};

}
}
