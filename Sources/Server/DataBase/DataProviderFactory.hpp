#pragma once

#include <memory>

#include "IDataProvider.hpp"

namespace Server
{
namespace DataBase
{

class DataProviderFactory
{
public:
    std::shared_ptr<IDataProvider> create(const std::string & type, const std::string & url);
};

}
}

