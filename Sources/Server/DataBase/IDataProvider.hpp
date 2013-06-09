#pragma once

#include "DataBaseNode.hpp"

namespace Server
{
namespace DataBase
{

class IDataProvider
{
public:
    virtual void load(DataBaseNode &) = 0;
    virtual void save(DataBaseNode &) = 0;

    virtual ~IDataProvider() {}
};

}
}
