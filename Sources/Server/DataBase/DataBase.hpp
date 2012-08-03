#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <memory>

#include "DataBase/DataBaseNode.hpp"

namespace Common 
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

#endif

