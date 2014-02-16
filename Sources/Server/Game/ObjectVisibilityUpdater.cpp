#include "ObjectVisibilityUpdater.hpp"

namespace Server
{
namespace Game
{

void ObjectVisibilityUpdater::sendVisibleObjects(const Common::Game::Position & position)
{
    Common::Messages::VisibleObjects visibleObjects;

    m_universe->objectsInProximity(position, 1000, [&](Common::Game::Object::ObjectBase & object)
    {
        if (object.is<Common::Game::Object::OwnedObjectBase>())
        {
            auto & ownedObject = dynamic_cast<Common::Game::Object::OwnedObjectBase &>(object);
            const unsigned ownerId = ownedObject.getOwnerId();
            m_playerContainer->invokeOnPlayer(ownerId, [&](Common::Game::IPlayer &, Network::IConnection & connection)
            {
                connection.send(visibleObjects);
            });
        }
    });
}

void ObjectVisibilityUpdater::sendVisibleObjectsByPlayer(unsigned playerId)
{
    Common::Messages::VisibleObjects visibleObjects;

    m_universe->objectsOwnedByPlayer(playerId, [&](Common::Game::Object::ObjectBase & object)
    {
        const auto position = object.getPosition();

        m_universe->objectsInProximity(position, 1000, [&](Common::Game::Object::ObjectBase & object)
        {
            if (object.is<Common::Game::Object::OwnedObjectBase>())
            {

            }
        });

        sendVisibleObjects(position);
    });

    m_playerContainer->invokeOnPlayer(playerId, [&](Common::Game::IPlayer &, Network::IConnection & connection)
    {
        connection.send(visibleObjects);
    });
}

}
}
