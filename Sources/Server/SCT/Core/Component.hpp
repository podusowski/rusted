#pragma once

#include <soci.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>

#include "Connection.hpp"

namespace SCT
{

class Component
{
public:
    Component(const std::string & sociDataBase = "sqlite3://SctDataBase.sqlite3");
    ~Component();

    void setConfigValue(const std::string & name, const std::string & value);
    void start();
    boost::shared_ptr<Connection> createConnection();
    boost::shared_ptr<soci::session> createSociSession();

private:
    pid_t m_pid;
    int m_port;
    std::map<std::string, std::string> m_cmdLineOptions;
    std::string m_sociDataBase;
};

}

