#pragma once

#include <soci.h>

#include "Cake/DependencyInjection/Inject.hpp"

#include "IShipClassContainer.hpp"
#include "ShipClass.hpp"

namespace Server
{
namespace Game
{

class ShipClassContainer : public IShipClassContainer
{
public:
    ShipClassContainer();
    ~ShipClassContainer();

    IShipClass & getById(unsigned) const;

private:
    void loadFromDataBase();

    std::map<unsigned, std::shared_ptr<ShipClass> > m_shipClassMap;

    Cake::DependencyInjection::Inject<soci::session> m_sociSession;
};

}
}
