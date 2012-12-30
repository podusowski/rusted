#include "ObjectBase.hpp"

using namespace Common::Game::Object;

ObjectBase::ObjectBase() :
    m_id(0),
    m_integrity(100)
{
}

unsigned ObjectBase::getId()
{
    return m_id;
}

void ObjectBase::setId(unsigned id)
{
    m_id = id;
}

void ObjectBase::setMesh(std::string mesh)
{
    m_mesh = mesh;
}

std::string ObjectBase::getMesh()
{
    return m_mesh;
}

unsigned ObjectBase::getIntegrity()
{
    return m_integrity;
}

void ObjectBase::setIntegrity(unsigned integrity)
{
    m_integrity = integrity;
}

Common::Math::Quaternion ObjectBase::getOrientation()
{
    return Common::Math::Quaternion();
}
