#pragma once

#include <boost/optional.hpp>
#include <boost/signals2.hpp>

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
    typedef boost::signals2::signal<void(std::vector<Common::Messages::AvailableAction>)> AvailableActionsFetchedSignal;
    typedef boost::signals2::signal<void()> GlobalCooldownActivatedSignal;
    typedef boost::signals2::signal<void()> GlobalCooldownExpiredSignal;
    typedef boost::signals2::signal<void(unsigned /* attacker */, unsigned /* attacked */)> ObjectAttackedSignal;

    PlayerActionService(Network::IConnection &, Common::Game::Player &, Common::Game::Universe &);

    boost::signals2::connection addAvailableActionsFetchedSlot(AvailableActionsFetchedSignal::slot_type);
    boost::signals2::connection addGlobalCooldownActivatedSlot(GlobalCooldownExpiredSignal::slot_type);
    boost::signals2::connection addGlobalCooldownExpiredSlot(GlobalCooldownExpiredSignal::slot_type);
    boost::signals2::connection addObjectAttackedSlot(ObjectAttackedSignal::slot_type);

    void focusObject(Common::Game::Object::ObjectBase &);
    void setFocusedObjectCourse(Common::Game::Position);
    void selectObject(Common::Game::Object::ObjectBase &);
    void fetchAvailableActions();
    void executeAction(unsigned actionId, unsigned actionParameter);

    void handle(const Common::Messages::AvailableActions &);
    void handle(const Common::Messages::GlobalCooldownActivated &);
    void handle(const Common::Messages::GlobalCooldownExpired &);
    void handle(const Common::Messages::AttackObject &);

    void handle(const Common::Messages::AbstractMessage &) {}

private:
    Common::Game::Player & m_player;
    Client::Network::IConnection & m_connection;
    Cake::DependencyInjection::Inject<Common::Game::IRustedTime> m_time;
    Common::Game::Universe & m_universe;

    boost::optional<Common::Game::Object::ObjectBase *> m_selectedObject;

    AvailableActionsFetchedSignal m_availableActionsFetchedSignal;
    GlobalCooldownActivatedSignal m_globalColldownActivatedSignal;
    GlobalCooldownExpiredSignal m_globalColldownExpiredSignal;
    ObjectAttackedSignal m_objectAttackedSignal;
};

}
}
