#include "XmlDataProvider.hpp"
#include "DataProviderFactory.hpp"

using namespace Server::DataBase;

std::shared_ptr<IDataProvider> DataProviderFactory::create(const std::string & type, const std::string & url)
{
    if (type == "xml")
    {
        return std::make_shared<XmlDataProvider>(url);
    }

    throw std::out_of_range("no such IDataProvider");
}
