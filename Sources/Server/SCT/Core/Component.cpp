#include <cstdio>
#include <vector>
#include <string>
#include <iostream>
#include <cstdlib>
#include <signal.h>
#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>

#include "Common/Thread.hpp"
#include "Component.hpp"
#include "Common/Logger/Logger.hpp"

using namespace ::SCT;

static int s_port = 2000;

Component::Component()
{
    std::cout << "\n";
}

Component::Component(const std::string & cfg) : m_cfg(cfg)
{
    std::cout << "\n";
}

Component::~Component()
{
/*
    std::cout << "SCT: closing connections (count: " << m_connections.size() << ")\n";

    BOOST_FOREACH(Connection * connection, m_connections)
    {
        delete connection;
    }
 */
    LOG_SCT << "killing application, pid: " << m_pid << "\n";
    ::kill(m_pid, 15);
    ::Common::Thread::wait(1);
}

void Component::setConfigValue(const std::string & name, const std::string & value)
{
    LOG_SCT << "passing config parameter to the application, " << name << ": " << value << "\n";
    m_cmdLineOptions[name] = value;
}

Connection & Component::createConnection()
{
    LOG_SCT << "creating new connection\n";
    Connection * connection = new Connection("127.0.0.1", m_port);
    m_connections.push_back(connection);
    return *connection;
}

void Component::start()
{
    //m_port = rand() % 1000 + 1025;
    m_port = s_port ++;

    const std::string filename = "./Server";

    // start the component
    m_pid = ::fork();
    if (!m_pid)
    {
        setConfigValue("--network.port", boost::lexical_cast<std::string>(m_port));
        if (!m_cfg.empty())
        {
            setConfigValue("--cfg", m_cfg);
        }

        char ** argv = new char*[m_cmdLineOptions.size() * 2 + 2];
        argv[0] = const_cast<char *>(filename.c_str());
        argv[m_cmdLineOptions.size() * 2 + 1] = 0;

        int i = 1;
        for (std::map<std::string, std::string>::iterator it = m_cmdLineOptions.begin();
             it != m_cmdLineOptions.end(); it++)
        {
            argv[i] = const_cast<char *>(it->first.c_str());
            i++;
            argv[i] = const_cast<char *>(it->second.c_str());
            i++;
        }

        ::execv(filename.c_str(), argv);
        LOG_SCT << "execv returned with error, errno: " << errno << "\n";
    }
    else
    {
        LOG_SCT << "running \"" << filename << "\" with, pid: " << m_pid << "\n";
    }

    ::Common::Thread::wait(1);
}

