#pragma once

#include <string>
#include <stack>

#include <libxml/parser.h>
#include <libxml/xmlwriter.h>

#include "DataBase/DataBaseNode.hpp"
#include "IDataProvider.hpp"

namespace Server 
{
namespace DataBase
{

class XmlDataProvider : public IDataProvider
{
public:
    XmlDataProvider(const std::string & xmlFile);
    void load(DataBaseNode &);
    void save(DataBaseNode &);

private:
    std::string m_xmlFile;
    xmlSAXHandler m_saxHandlersTable;
    std::stack<DataBaseNode *> m_stack;

    void saveNode(DataBaseNode &, xmlTextWriterPtr);

    static void startElement(void * ctx, const xmlChar * name, const xmlChar ** atts);
    static void endElement(void * ctx, const xmlChar * name);
};

}
}

