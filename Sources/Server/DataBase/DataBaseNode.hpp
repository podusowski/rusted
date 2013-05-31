#pragma once

#include <string>
#include <map>
#include <vector>
#include <stdexcept>
#include <memory>

#include <boost/lexical_cast.hpp>

namespace Server 
{
namespace DataBase
{

class DataBaseNode
{
public:
    typedef std::vector<std::shared_ptr<DataBaseNode>> NodeVector;

    DataBaseNode(const std::string & name);
    DataBaseNode & createChild(const std::string & name);
    DataBaseNode & getFirstChild(const std::string & name);
    NodeVector getChilds();
    size_t getChildCount();

    template <typename T>
    void setValue(const std::string & name, const T value)
    {
        m_values[name] = boost::lexical_cast<std::string>(value);
    }

    template <typename T> 
    const T getValue(const std::string & name)
    {
        if (m_values.find(name) == m_values.end())
            throw std::out_of_range("there is no value \"" + name + "\" in node \"" + m_name + "\"");
        return boost::lexical_cast<T>(m_values[name]);
    }

    template <typename T> 
    const T getValue(const std::string & name, const T & defaultValue)
    {
        if (m_values.find(name) == m_values.end())
        {
            return defaultValue;
        }
        else
        {
            return boost::lexical_cast<T>(m_values[name]);
        }
    }

private:
    std::string m_name;
    std::map<std::string, std::string> m_values;
    NodeVector m_childs;
};

}
}

