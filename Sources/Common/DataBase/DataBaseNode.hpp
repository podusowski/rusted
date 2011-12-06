#pragma once

#include <string>
#include <map>
#include <vector>
#include <stdexcept>

#include <boost/lexical_cast.hpp>

namespace Common 
{
namespace DataBase
{

class DataBaseNodeSelection;

class DataBaseNode
{
public:
    typedef std::vector<DataBaseNode *>::iterator iterator;

    DataBaseNode(const std::string & name);
    ~DataBaseNode();
    DataBaseNode & createChild(const std::string & name);
    DataBaseNode & getFirstChild(const std::string & name);
    DataBaseNodeSelection getChilds();
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

private:
    std::string m_name;
    std::map<std::string, std::string> m_values;
    std::vector<DataBaseNode *> m_childs;
};

class DataBaseNodeSelection
{
public:
    DataBaseNodeSelection(std::vector<DataBaseNode *> & nodes);
    DataBaseNode::iterator begin();
    DataBaseNode::iterator end();

private:
    std::vector<DataBaseNode *> & m_nodes;
};

}
}

