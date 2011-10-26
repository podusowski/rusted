#include <stdexcept>
#include <boost/foreach.hpp>

#include "Common/Logger/Logger.hpp"
#include "Server/Game/StaticObjectContainer.hpp"

using namespace Server::Game;

StaticObjectContainer::StaticObjectContainer(Common::DataBase::DataBase & db) : m_db(db)
{
    try
    {
        loadFromDataBase();
    }
    catch (std::out_of_range ex)
    {
        LOG_WARN << "Can't load static objects from db (reason: " << ex.what() << ", empty node will be created\n";
    }
}

void StaticObjectContainer::loadFromDataBase()
{
    using Common::Game::StaticObject;

    LOG_INFO << "Loading static objects\n";

    Common::DataBase::DataBaseNode & staticObjectsNode = m_db.getRoot().getFirstChild("staticObjects");
    for (Common::DataBase::DataBaseNode::iterator it = staticObjectsNode.getChilds().begin();
         it != staticObjectsNode.getChilds().end(); it++)
    {
        Common::DataBase::DataBaseNode & staticObjectNode = **it;
        StaticObject * staticObject = new StaticObject(staticObjectNode.getValue<int>("id"),
                                                       Common::Game::StaticObject::Position(
                                                           staticObjectNode.getValue<int>("x"),
                                                           staticObjectNode.getValue<int>("y"),
                                                           staticObjectNode.getValue<int>("z")
                                                       ));
        m_staticObjects.push_back(staticObject);
    }
}

StaticObjectContainer::StaticObjectVector & StaticObjectContainer::getStaticObjects()
{
    return m_staticObjects;
}

Common::Game::StaticObject & StaticObjectContainer::getStaticObject(int id)
{
    BOOST_FOREACH(Common::Game::StaticObject & object, m_staticObjects)
    {
        if (object.getId() == id)
        {
            return object;
        }
    }
    throw std::out_of_range("no such static object");
}
