#pragma once

#include <memory>

#include "DataBase/DataBaseNode.hpp"

namespace Server
{
namespace DataBase
{

class DataBase
{
public:
    DataBase();
    DataBaseNode & getRoot();
    void reset();

private:
    std::auto_ptr<DataBaseNode> m_root;
};

}
}

