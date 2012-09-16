#pragma once

#include <boost/shared_ptr.hpp>

#include "Common/Game/Object/ObjectBase.hpp"
#include "DataBase/DataBase.hpp"

namespace Server
{
namespace Game
{

class IObjectFactory
{
public:
    virtual boost::shared_ptr<Common::Game::Object::ObjectBase> create(Server::DataBase::DataBaseNode &) = 0;
    virtual boost::shared_ptr<Common::Game::Object::ObjectBase> createShip(unsigned shipClass, unsigned ownerId) = 0;

    virtual ~IObjectFactory() {}
};

}
}

