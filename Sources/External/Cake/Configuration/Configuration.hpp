#pragma once

#include <stdexcept>
#include <string>
#include <map>
#include <boost/lexical_cast.hpp>

namespace Cake 
{
namespace Configuration
{

class Configuration
{
public:
    Configuration(int argc, const char * argv[]);
    Configuration();

    template <typename T>
    const T getValue(const std::string & name)
    {
        if (m_properties.find(name) == m_properties.end())
            throw std::out_of_range("no such property " + name);
        return boost::lexical_cast<T>(m_properties[name]);
    }

    template <typename T>
    const T getValue(const std::string & name, const T & def)
    {
        if (m_properties.find(name) == m_properties.end())
            return def;
        return boost::lexical_cast<T>(m_properties[name]);
    }

    template <typename T>
    void setValue(const std::string & name, const T & value)
    {
        m_properties[name] = boost::lexical_cast<std::string>(value);
    }

    bool hasValue(const std::string & name);

    std::string getAppName();

private:
    void parse(const std::string & filename);
    void parse(int argc, const char * argv[]);

    std::map<std::string, std::string> m_properties;
    std::string m_appName;
};

}
}

