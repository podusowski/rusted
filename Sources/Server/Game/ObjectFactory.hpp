#pragma once

#include <boost/shared_ptr.hpp>

#include "Common/Game/Object/ObjectBase.hpp"
#include "DataBase/DataBase.hpp"
#include "IShipClassContainer.hpp"
#include "IObjectFactory.hpp"

namespace Server
{
namespace Game
{

class ObjectFactory : public IObjectFactory
{
public:
	ObjectFactory(DataBase::DataBase & db, IShipClassContainer &);
	boost::shared_ptr<Common::Game::Object::ObjectBase> create(Server::DataBase::DataBaseNode & data);

private:
    Common::Game::Position extractPosition(Server::DataBase::DataBaseNode & data);

    DataBase::DataBase & m_db;
    IShipClassContainer & m_shipClassContainer;
};

}
}

