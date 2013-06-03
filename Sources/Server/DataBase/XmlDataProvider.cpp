#include <iostream>
#include <cstring>
#include <fstream>

#include "Cake/Diagnostics/Logger.hpp"

#include "DataBase/XmlDataProvider.hpp"

using namespace Server::DataBase;

XmlDataProvider::XmlDataProvider(DataBase & db, const std::string & xmlFile) : 
    m_db(db),
    m_xmlFile(xmlFile)
{
}

void XmlDataProvider::load()
{
    LOG_INFO << "Loading db from " << m_xmlFile;

    m_stack.push(&m_db.getRoot());

    memset(&m_saxHandlersTable, 0, sizeof(xmlSAXHandler));
    m_saxHandlersTable.startElement = &startElement;
    m_saxHandlersTable.endElement = &endElement;

    xmlSAXUserParseFile(&m_saxHandlersTable, &m_stack, m_xmlFile.c_str());
}

void XmlDataProvider::save()
{
    LOG_INFO << "Saving db to " << m_xmlFile;
    std::fstream f(m_xmlFile);

    f << "<root>\n";
    f << "</root>\n";
}

void XmlDataProvider::startElement(void * ctx, const xmlChar * name, const xmlChar ** atts)
{
    std::string s(reinterpret_cast<const char *>(name));
    if (s == "root") return;

    std::stack<DataBaseNode *> & stack = *static_cast<std::stack<DataBaseNode *> *>(ctx);

    DataBaseNode & node = stack.top()->createChild(reinterpret_cast<const char *>(name)); 
    stack.push(&node);

    std::string logPrefix;
    for (size_t i = 0; i < stack.size() -1; i++)
    {
        logPrefix += " |";
    }
    LOG_DEBUG << logPrefix << "-" << reinterpret_cast<const char *>(name);

    while (atts != 0 && *atts != 0)
    {
        LOG_DEBUG << logPrefix << "    " << reinterpret_cast<const char *>(atts[0]) 
                 << ": " << reinterpret_cast<const char *>(atts[1]);

        node.setValue(reinterpret_cast<const char *>(atts[0]),
                      reinterpret_cast<const char *>(atts[1]));
        atts += 2;
    }
}

void XmlDataProvider::endElement(void * ctx, const xmlChar * name)
{
    std::string s(reinterpret_cast<const char *>(name));
    if (s == "root") return;

    std::stack<DataBaseNode *> & stack = *static_cast<std::stack<DataBaseNode *> *>(ctx);
    stack.pop();
}

