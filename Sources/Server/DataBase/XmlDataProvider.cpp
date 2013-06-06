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

    int rc;
    xmlTextWriterPtr writer = xmlNewTextWriterFilename(m_xmlFile.c_str(), 0);

    if (!writer)
    {
        throw std::runtime_error("libxml error");
    }

    rc = xmlTextWriterStartDocument(writer, NULL, "UTF-8", NULL);
    if (rc < 0)
    {
        throw std::runtime_error("libxml error");
    }

    saveNode(m_db.getRoot(), writer);

    rc = xmlTextWriterEndDocument(writer);
    if (rc < 0)
    {
        throw std::runtime_error("libxml error");
    }

    xmlFreeTextWriter(writer);
}

void XmlDataProvider::saveNode(DataBaseNode & node, xmlTextWriterPtr writer)
{
    LOG_DEBUG << "  " << node.getName();

    int rc;

    rc = xmlTextWriterStartElement(writer, BAD_CAST node.getName().c_str());
    if (rc < 0)
    {
        throw std::runtime_error("libxml error");
    }

    auto values = node.getValues();

    for (auto value : values)
    {
        rc = xmlTextWriterWriteAttribute(writer,
            BAD_CAST value.first.c_str(),
            BAD_CAST value.second.c_str());
    }

    auto childs = node.getChilds();
    for (auto child : childs)
    {
        saveNode(*child, writer);
    }

    rc = xmlTextWriterEndElement(writer);
    if (rc < 0)
    {
        throw std::runtime_error("libxml error");
    }
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

