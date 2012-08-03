#pragma once

#include <string>
#include <stack>

#include <libxml2/libxml/parser.h>

#include "DataBase/DataBase.hpp"

namespace Server 
{
namespace DataBase
{

class XmlDataProvider
{
public:
    XmlDataProvider(DataBase & db, const std::string & xmlFile);

private:
    xmlSAXHandler m_saxHandlersTable;
    std::stack<DataBaseNode *> m_stack;
    DataBase & m_db;

    static void startElement(void * ctx, const xmlChar * name, const xmlChar ** atts);
    static void endElement(void * ctx, const xmlChar * name);
};

}
}

