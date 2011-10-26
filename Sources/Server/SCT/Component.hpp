#ifndef SCT_COMPONENT_HPP
#define SCT_COMPONENT_HPP

#include <unistd.h>

#include <string>
#include <vector>

#include <SCT/Connection.hpp>

namespace SCT
{

class Component
{
public:
	Component();
    Component(const std::string & cfg);
	~Component();
    void setConfigValue(const std::string & name, const std::string & value);
    void start();
	Connection & createConnection();

private:
	std::vector<Connection *> m_connections;
	pid_t m_pid;
	int m_port;
    std::string m_cfg;
    std::map<std::string, std::string> m_cmdLineOptions;
};

}

#endif
