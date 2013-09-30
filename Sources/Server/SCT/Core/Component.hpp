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
    Component(const std::string & sqliteUrl = "SctDataBase.sqlite3");
    ~Component();

    void setConfigValue(const std::string & name, const std::string & value);
    void start();
    std::shared_ptr<Connection> createConnection();
    std::shared_ptr<soci::session> createSociSession();

private:
    std::string copySqliteDb(const std::string & db);
    void removeDb();

    pid_t m_pid;
    int m_port;
    std::map<std::string, std::string> m_cmdLineOptions;
    std::string m_sqliteUrl;
};

}

