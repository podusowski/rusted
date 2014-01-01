#include <fstream>

#include "Diagnostics/Logger.hpp"
#include "Configuration.hpp"

using namespace Cake::Configuration;

enum ConfigurationParserState
{
    ConfigurationParserState_Iddle,
    ConfigurationParserState_ReadingName,
    ConfigurationParserState_WaitingForValue,
    ConfigurationParserState_ReadingValue
};

Configuration::Configuration(int argc, const char * argv[])
{
    parse(argc, argv);
}

Configuration::Configuration()
{
    LOG_INFO << "Initializing empty configuration";
}

void Configuration::parse(const std::string & filename)
{
    LOG_INFO << "Parsing configuration file \"" << filename << "\"";

    std::ifstream file(filename.c_str());

    if (!file.good())
    {
        LOG_ERR << "There was error while opening configuration file, syntax might be wrong or file missing?";
        return;
    }

    ConfigurationParserState state = ConfigurationParserState_ReadingName; 
    std::string name;
    std::string value;

    while (file.good())
    {
        char c;
        file.get(c);

        switch (state)
        {
        case ConfigurationParserState_Iddle:
            state = ConfigurationParserState_ReadingName;
            break;

        case ConfigurationParserState_ReadingName:

            switch (c)
            {
            case ' ':
            case '\t':
                state = ConfigurationParserState_WaitingForValue;
                break;
            case '=':
                state = ConfigurationParserState_ReadingValue;
                break;
            default:
                name += c;
                break;
            }
            break;

        case ConfigurationParserState_WaitingForValue:

            switch (c)
            {
            case '=':
                state = ConfigurationParserState_ReadingValue;
                break;
            }
            break;

        case ConfigurationParserState_ReadingValue:

            switch (c)
            {
            case '\n':
            case '#':
                LOG_DEBUG << "  option: " << name
                          << ", value: " << value;
 
                m_properties[name] = value; 
                name.clear();
                value.clear();
                state = ConfigurationParserState_ReadingName;
                break;
            default:
                value += c;
                break;
            }
            break;
        }    
    }
    LOG_DEBUG << "Configuration file parsed";
}

void Configuration::parse(int argc, const char * argv[])
{
    LOG_DEBUG << "Parsing program's cmd line";

    if (exists("config.cfg"))
    {
        parse("config.cfg");
    }

    std::map<std::string, std::string> cmdLineOptions;

    m_appName = argv[0];

    for (int i = 0; i < argc; i++)
    {
        if (argv[i][0] == '-' && i+1 < argc)
        {
            LOG_DEBUG << "  option: " << argv[i] <<
                      ", value: " << argv[i+1];

            std::string name = argv[i];
            if (name == "--cfg")
            {
                parse(argv[i+1]);
            }
            else
            {
                if (name.size() > 2)
                {
                    name.erase(0, 2);
                    cmdLineOptions[name] = argv[i+1];
                }
            }
        }
    }

    for (std::map<std::string, std::string>::iterator it = cmdLineOptions.begin();
            it != cmdLineOptions.end(); it++)
    {
        m_properties[it->first] = it->second;
    }
}

bool Configuration::hasValue(const std::string & name)
{
    return m_properties.find(name) != m_properties.end();
}

std::string Configuration::getAppName()
{
    return m_appName;
}

bool Configuration::exists(std::string file)
{
    std::ifstream f(file);
    if (f.good())
    {
        return true;
    }
    else
    {
        return false;
    }
}

