#include "Universe.hpp"

using namespace Common::Game;

void Universe::loadFromDataBase(DataBase::DataBase & db)
{
    try
    {
        DataBase::DataBaseNode & objects = db.getRoot().getFirstChild("objects");

        for (DataBase::DataBaseNode::iterator it = objects.getChilds().begin();
             it != objects.getChilds().end(); it++)
        {
            DataBase::DataBaseNode & objectData = **it;
            boost::shared_ptr<Object::IObject> object = m_objectFactory->createObject(objectData);
        }
    }
    catch (...)
    {
        // TODO: log
    }
}
