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
    CAKE_LOG_INFO << "Initializing empty configuration\n";
}

void Configuration::parse(const std::string & filename)
{
    CAKE_LOG_INFO << "Parsing configuration file \"" << filename << "\"\n";

    std::ifstream file(filename.c_str());

    if (!file.good())
    {
        CAKE_LOG_ERR << "There was error while opening configuration file, syntax might be wrong or file missing?\n";
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
                CAKE_LOG_INFO << "  option: " << name
                         << ", value: " << value << "\n";
 
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
    CAKE_LOG_INFO << "Configuration file parsed\n";
}

void Configuration::parse(int argc, const char * argv[])
{
    CAKE_LOG_INFO << "Parsing program's cmd line\n";
    std::map<std::string, std::string> cmdLineOptions;

    m_appName = argv[0];

    for (int i = 0; i < argc; i++)
    {
        if (argv[i][0] == '-' && i+1 < argc)
        {
            CAKE_LOG_INFO << "  option: " << argv[i] <<
                    ", value: " << argv[i+1] << "\n";
            
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

std::string Configuration::getAppName()
{
	return m_appName;
}

