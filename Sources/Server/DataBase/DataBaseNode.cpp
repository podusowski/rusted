#include <stdexcept>
#include <boost/foreach.hpp>

#include "DataBase/DataBaseNode.hpp"

using namespace Server::DataBase;

DataBaseNode::DataBaseNode(const std::string & name) : m_name(name)
{
}

std::string DataBaseNode::getName() const
{
    return m_name;
}

DataBaseNode & DataBaseNode::createChild(const std::string & name)
{
    auto node = std::shared_ptr<DataBaseNode>(new DataBaseNode(name));
    m_childs.push_back(node);
    return *node;
}

DataBaseNode & DataBaseNode::getFirstChild(const std::string & name)
{
    for (auto node : m_childs)
    {
        if (node->m_name == name)
        {
            return *node;
        }
    }
    throw std::out_of_range("there is no child named \"" + name + "\" in node \"" + m_name + "\"");
}

DataBaseNode::NodeVector DataBaseNode::getChilds()
{
    return m_childs;
}

size_t DataBaseNode::getChildCount()
{
    return m_childs.size();
}

DataBaseNode::ValuesMap DataBaseNode::getValues() const
{
    return m_values;
}
