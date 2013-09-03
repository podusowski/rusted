#pragma once

#include <boost/shared_ptr.hpp>

#include <soci.h>

#include "Cake/DependencyInjection/Inject.hpp"

#include "Common/Game/Universe.hpp"
#include "Common/Game/Object/ObjectBase.hpp"
#include "IShipClassContainer.hpp"
#include "IUniverseDataBaseFacade.hpp"

namespace Server
{
namespace Game
{

class UniverseDataBaseFacade : public IUniverseDataBaseFacade
{
public:
    UniverseDataBaseFacade(IShipClassContainer &);
    boost::shared_ptr<Common::Game::Object::ObjectBase> create(const soci::row &);
    boost::shared_ptr<Common::Game::Object::ObjectBase> createShip(unsigned shipClass, unsigned ownerId);

private:
    void loadFromDb();
    Common::Game::Position extractPosition(const soci::row &);
    void fillCargoHold(const soci::row &, Common::Game::Object::CargoHold &);
    unsigned preInsertObjectToDb(unsigned shipClassId, unsigned ownerId);

    IShipClassContainer & m_shipClassContainer;
    Cake::DependencyInjection::Inject<Common::Game::Universe> m_universe;
    Cake::DependencyInjection::Inject<soci::session> m_sociSession;
};

}
}
