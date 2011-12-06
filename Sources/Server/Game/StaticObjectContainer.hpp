#pragma once

#include <boost/ptr_container/ptr_vector.hpp>

#include "Common/DataBase/DataBase.hpp"
#include "Common/Game/StaticObject.hpp"

namespace Server
{
namespace Game
{

class StaticObjectContainer
{
public:
    typedef boost::ptr_vector<Common::Game::StaticObject> StaticObjectVector;

    StaticObjectContainer(Common::DataBase::DataBase &);
    StaticObjectVector & getStaticObjects();
    Common::Game::StaticObject & getStaticObject(int id);

private:
    void loadFromDataBase();

    Common::DataBase::DataBase & m_db;
    StaticObjectVector m_staticObjects;
};

}
}
