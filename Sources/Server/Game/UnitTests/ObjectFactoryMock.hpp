#pragma once

#include <gmock/gmock.h>

#include "Game/IObjectFactory.hpp"

namespace Server
{
namespace Game
{

class ObjectFactoryMock : public IObjectFactory
{
public:
	MOCK_METHOD1(create, boost::shared_ptr<Common::Game::Object::ObjectBase>(Server::DataBase::DataBaseNode &));
};

}
}

