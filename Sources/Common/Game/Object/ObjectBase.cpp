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

void ObjectBase::setModel(std::string model)
{
    m_model = model;
}

std::string ObjectBase::getModel()
{
    return m_model;
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

void ObjectBase::setOrientation(const Common::Math::Quaternion &)
{
}

void ObjectBase::writeCargoHold(std::function<void(CargoHold &)> function)
{
    function(m_cargoHold);
    m_cargoHoldSignal(m_cargoHold);
}

void ObjectBase::readCargoHold(std::function<void(const CargoHold &)> function) const
{
    function(m_cargoHold);
}

boost::signals2::connection ObjectBase::addCargoHoldSlot(CargoHoldSignal::slot_type slot)
{
    return m_cargoHoldSignal.connect(slot);
}

ObjectBase::~ObjectBase()
{
}

