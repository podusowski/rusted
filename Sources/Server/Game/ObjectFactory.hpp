#pragma once

#include <boost/shared_ptr.hpp>

#include <soci.h>

#include "Common/Game/Object/ObjectBase.hpp"
#include "IShipClassContainer.hpp"
#include "IObjectFactory.hpp"

namespace Server
{
namespace Game
{

class ObjectFactory : public IObjectFactory
{
public:
    ObjectFactory(IShipClassContainer &);
    boost::shared_ptr<Common::Game::Object::ObjectBase> create(const soci::row &);
    boost::shared_ptr<Common::Game::Object::ObjectBase> createShip(unsigned shipClass, unsigned ownerId);

private:
    Common::Game::Position extractPosition(const soci::row &);
    void fillCargoHold(const soci::row &, Common::Game::Object::CargoHold &);

    IShipClassContainer & m_shipClassContainer;
};

}
}

