#include <cstdio>
#include <vector>
#include <string>
#include <iostream>
#include <cstdlib>
#include <signal.h>
#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>

#include "Cake/Diagnostics/Logger.hpp"
#include "Cake/Threading/Thread.hpp"

#include "Component.hpp"

using namespace ::SCT;

static int s_port = 2000;

Component::Component()
{
    std::cout << "\n";
}

Component::Component(const std::string & database)
{
    setConfigValue("--database.provider1.type", "xml");
    setConfigValue("--database.provider1.url", database);

    std::cout << "\n";
}

Component::~Component()
{
    LOG_INFO << "Killing application, pid: " << m_pid;

    ::kill(m_pid, 15);
    Cake::Threading::Thread::wait(0, 500);
}

void Component::setConfigValue(const std::string & name, const std::string & value)
{
    LOG_INFO << "Passing config parameter to the application, " << name << ": " << value;

    m_cmdLineOptions[name] = value;
}

boost::shared_ptr<Connection> Component::createConnection()
{
    LOG_INFO << "Creating new connection";

    return boost::shared_ptr<Connection>(new Connection("127.0.0.1", m_port));
}

void Component::start()
{
    //m_port = rand() % 1000 + 1025;
    m_port = s_port ++;

    if (getenv("SERVER_SCT_PORT"))
    {
        m_port = boost::lexical_cast<unsigned>(getenv("SERVER_SCT_PORT"));
        LOG_INFO << "Port overwritten to " << m_port << " by SERVER_SCT_PORT environment variable";
    }

    const std::string filename = "./Server";

    // start the component
    m_pid = ::fork();
    if (!m_pid)
    {
        setConfigValue("--network.port", boost::lexical_cast<std::string>(m_port));

        std::stringstream administrationSocketPath;
        administrationSocketPath << "/var/tmp/rusted_sct_" << m_port;
        setConfigValue("--network.administration_socket_path", administrationSocketPath.str());

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
        LOG_ERR << "execv returned with error, errno: " << errno;
    }
    else
    {
        LOG_INFO << "Running \"" << filename << "\", pid: " << m_pid;
    }

    // usefull for valgrind
    if (getenv("SERVER_SCT_WAIT_FOR_APP_TIME"))
    {
        unsigned seconds = boost::lexical_cast<unsigned>(getenv("SERVER_SCT_WAIT_FOR_APP_TIME"));
        Cake::Threading::Thread::wait(seconds);
    }
    else
    {
        Cake::Threading::Thread::wait(0, 500);
    }

    #if 0
    for (int i = 0; i < 100; i++)
    {
        try
        {
            Connection testConnection("127.0.0.1", m_port);
        }
        catch (...)
        {
            Cake::Threading::Thread::wait(0, 500);
        }
    }
    #endif
}

