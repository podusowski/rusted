#include "DataBase/DataBase.hpp"

using namespace Common::DataBase;

DataBase::DataBase()
{
    reset();
}

DataBaseNode & DataBase::getRoot()
{
    return *m_root;
}

void DataBase::reset()
{
    m_root.reset(new DataBaseNode("root"));
}
