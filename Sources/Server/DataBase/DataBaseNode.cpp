#include <stdexcept>
#include <boost/foreach.hpp>

#include "DataBase/DataBaseNode.hpp"

using namespace Server::DataBase;

DataBaseNode::DataBaseNode(const std::string & name) : m_name(name)
{
}

DataBaseNode::~DataBaseNode()
{
    BOOST_FOREACH(DataBaseNode * node, m_childs)
    {
        delete node;
    }
}

DataBaseNode & DataBaseNode::createChild(const std::string & name)
{
    DataBaseNode * node = new DataBaseNode(name);
    m_childs.push_back(node);
    return *node;
}

DataBaseNode & DataBaseNode::getFirstChild(const std::string & name)
{
    BOOST_FOREACH(DataBaseNode * node, m_childs)
    {
        if (node->m_name == name)
        {
            return *node;
        }
    }
    throw std::out_of_range("there is no child named \"" + name + "\" in node \"" + m_name + "\"");
}

DataBaseNodeSelection DataBaseNode::getChilds()
{
    DataBaseNodeSelection selection(m_childs);
    return selection;
}

size_t DataBaseNode::getChildCount()
{
    return m_childs.size();
}

DataBaseNodeSelection::DataBaseNodeSelection(std::vector<DataBaseNode *> & nodes) : m_nodes(nodes)
{
}

DataBaseNode::iterator DataBaseNodeSelection::begin()
{
    return m_nodes.begin();
}

DataBaseNode::iterator DataBaseNodeSelection::end()
{
    return m_nodes.end();
}
