#include "Cake/Diagnostics/Logger.hpp"

#include "Common/Game/Object/Ship.hpp"
#include "Client/Services/PlayerActionService.hpp"

using namespace Client::Services;

PlayerActionService::PlayerActionService(Network::IConnection & connection, 
                             Common::Game::Player & player,
                             Common::Game::Universe & universe) :
    m_player(player),
    m_connection(connection),
    m_universe(universe)
{
}

boost::signals2::connection PlayerActionService::addAvailableActionsFetchedSlot(AvailableActionsFetchedSignal::slot_type slot)
{
    return m_availableActionsFetchedSignal.connect(slot);
}

boost::signals2::connection PlayerActionService::addGlobalCooldownActivatedSlot(GlobalCooldownActivatedSignal::slot_type slot)
{
    return m_globalColldownActivatedSignal.connect(slot);
}

boost::signals2::connection PlayerActionService::addGlobalCooldownExpiredSlot(GlobalCooldownExpiredSignal::slot_type slot)
{
    return m_globalColldownExpiredSignal.connect(slot);
}

boost::signals2::connection PlayerActionService::addObjectAttackedSlot(ObjectAttackedSignal::slot_type slot)
{
    return m_objectAttackedSignal.connect(slot);
}

void PlayerActionService::focusObject(Common::Game::Object::ObjectBase & object)
{
    LOG_DEBUG << "Object focused: " << object.getId();

    Common::Messages::FocusObject focusObject;
    focusObject.id = object.getId();
    m_connection.send(focusObject);

    m_player.focusObject(object);
}

void PlayerActionService::setFocusedObjectCourse(Common::Game::Position course)
{
    auto & focusedShip = dynamic_cast<Common::Game::Object::Ship &>(m_player.getFocusedObject());

    //focusedShip.setCourse(course);

    Common::Messages::ChangeShipCourse req;

    req.shipId = focusedShip.getId();
    req.x = course.getX();
    req.y = course.getY();
    req.z = course.getZ();

    m_connection.send(req);
}

void PlayerActionService::selectObject(Common::Game::Object::ObjectBase & object)
{
    LOG_DEBUG << "Object selected: " << object.getId();

    Common::Messages::SelectObject selectObject;
    selectObject.id = object.getId();
    m_connection.send(selectObject);

    m_selectedObject = &object;
    m_player.selectObject(object);
}

void PlayerActionService::fetchAvailableActions()
{
    LOG_DEBUG << "Fetching available actions";

    auto & focusedShip = dynamic_cast<Common::Game::Object::Ship &>(m_player.getFocusedObject());

    Common::Messages::FetchAvailableActions fetchAvailableActions;
    fetchAvailableActions.shipId = focusedShip.getId();
    m_connection.send(fetchAvailableActions);
}

void PlayerActionService::executeAction(unsigned actionId, unsigned actionParameter, bool loop)
{
    LOG_DEBUG << "Executing action: " << actionId;

    Common::Messages::ExecuteAction executeAction;
    executeAction.id = actionId;
    executeAction.parameter = actionParameter;
    executeAction.loop = loop;

    m_connection.send(executeAction);
}

void PlayerActionService::handle(const Common::Messages::AvailableActions & availableActions)
{
    LOG_DEBUG << "Got available actions";

    m_availableActionsFetchedSignal(availableActions.actions);
}

// TODO: global cooldown is per ship?
void PlayerActionService::handle(const Common::Messages::GlobalCooldownActivated & globalCooldownActivated)
{
    LOG_DEBUG << "Global cooldown activated";

    m_globalColldownActivatedSignal();
}

void PlayerActionService::handle(const Common::Messages::GlobalCooldownExpired &)
{
    LOG_DEBUG << "Global cooldown expired";

    m_globalColldownExpiredSignal();
}

void PlayerActionService::handle(const Common::Messages::AttackObject & attackObject)
{
    LOG_DEBUG << "Ship:" << attackObject.attackedId << " is attacked by ship:" << attackObject.attackerId;;

    m_objectAttackedSignal(attackObject.attackerId, attackObject.attackedId);
}

