#pragma once

#include <boost/shared_ptr.hpp>

#include "Common/Game/Object/ObjectBase.hpp"
#include "DataBase/DataBase.hpp"

namespace Server
{
namespace Game
{

class ObjectFactory
{
public:
	ObjectFactory(DataBase::DataBase & db);
	boost::shared_ptr<Common::Game::Object::ObjectBase> create(Server::DataBase::DataBaseNode & data);

private:
    Common::Game::Position extractPosition(Server::DataBase::DataBaseNode & data);
    DataBase::DataBaseNode & getShipClass(unsigned id);

    DataBase::DataBase & m_db;
};

}
}
