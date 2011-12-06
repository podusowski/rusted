#pragma once

#include <vector>
#include <boost/shared_ptr.hpp>

#include "Cake/DependencyInjection/Inject.hpp"

#include "Object/ObjectFactory.hpp"
#include "Game/Object/IObject.hpp"
#include "DataBase/DataBase.hpp"

namespace Common
{
namespace Game
{

class Universe
{
public:
    void loadFromDataBase(DataBase::DataBase & db);
    void add(boost::shared_ptr<Object::IObject> object);
    std::vector<boost::shared_ptr<Object::IObject> > getPlayerObjects(unsigned playerId);

private:
    std::vector<boost::shared_ptr<Object::IObject> > m_objects;
    Cake::DependencyInjection::Inject<Object::ObjectFactory> m_objectFactory;
};

}
}
