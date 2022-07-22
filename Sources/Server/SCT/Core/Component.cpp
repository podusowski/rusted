#include <cstdio>
#include <vector>
#include <string>
#include <iostream>
#include <cstdlib>
#include <signal.h>
#include <fstream>
#include <stdexcept>

#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>

#include "Cake/Diagnostics/Logger.hpp"
#include "Cake/Threading/Thread.hpp"
#include "Cake/Utils/BuildString.hpp"

#include "Component.hpp"
#include "Configuration.hpp"

using namespace ::SCT;

static int s_port = 2000;

Component::Component(const std::string & sqliteUrl,  const std::string& server_path): server_path{server_path}
{
    m_port = s_port ++;

    if (getenv("SERVER_SCT_PORT"))
    {
        m_port = boost::lexical_cast<unsigned>(getenv("SERVER_SCT_PORT"));
        LOG_INFO << "Port overwritten to " << m_port << " by SERVER_SCT_PORT environment variable";
    }

    m_sqliteUrl = copySqliteDb(sqliteUrl);
    m_administrationSocketPath = BUILD_STRING << "/tmp/rusted_sct_" << m_port;

    std::string sociUrl = "sqlite3://" + m_sqliteUrl;
    setConfigValue("--database.url", sociUrl);

    std::cout << "\n";
}

Component::~Component()
{
    LOG_INFO << "Killing application, pid: " << m_pid;

    ::kill(m_pid, 15);
    removeDb();

    Cake::Threading::Thread::wait(0, 500);
}

void Component::setConfigValue(const std::string & name, const std::string & value)
{
    LOG_INFO << "Passing config parameter to the application, " << name << ": " << value;

    m_cmdLineOptions[name] = value;
}

std::shared_ptr<Connection> Component::createConnection()
{
    LOG_INFO << "Creating player connection";

    return std::make_shared<Connection>(Connection::Type::TCP, "127.0.0.1", m_port);
}

std::shared_ptr<Connection> Component::createAdministrationConnection()
{
    LOG_INFO << "Creating administration connection";

    return std::make_shared<Connection>(Connection::Type::UNIX, m_administrationSocketPath, m_port);
}

std::shared_ptr<soci::session> Component::createSociSession()
{
    return std::shared_ptr<soci::session>(new soci::session("sqlite3://" + m_sqliteUrl));
}

void Component::start()
{
    // start the component
    m_pid = ::fork();
    if (!m_pid)
    {
        setConfigValue("--network.port", boost::lexical_cast<std::string>(m_port));
        setConfigValue("--network.administration_socket_path", m_administrationSocketPath);

        char ** argv = new char*[m_cmdLineOptions.size() * 2 + 2];
        argv[0] = const_cast<char *>(server_path.c_str());
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

        ::execv(server_path.c_str(), argv);
        LOG_ERR << "execv returned with error, errno: " << errno;
    }
    else
    {
        LOG_INFO << "Running \"" << server_path << "\", pid: " << m_pid;
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
}

std::string Component::copySqliteDb(const std::string & db)
{
    std::string ret = tmpnam(nullptr);

    LOG_INFO << "Copying database from: " << db << " to: " << ret;

    std::ifstream src(db, std::ios::binary);
    std::ofstream dst(ret, std::ios::binary);

    dst << src.rdbuf();

    return ret;
}

void Component::removeDb()
{
    LOG_INFO << "Deleting temporary db: " << m_sqliteUrl;
    std::remove(m_sqliteUrl.c_str());
}

namespace SCT{
Component make_default_component()
{
    using namespace SCT::Configuration;
    return Component(get_database_path(), get_sut_path());

}
}
