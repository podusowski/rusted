#pragma once

#include <stdexcept>
#include <string>
#include <map>
#include <sstream>

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
        return convert<T>(m_properties[name]);
    }

    template <typename T>
    const T getValue(const std::string & name, const T & def)
    {
        if (m_properties.find(name) == m_properties.end())
            return def;
        return convert<T>(m_properties[name]);
    }

    template <typename T>
    void setValue(const std::string & name, const T & value)
    {
        m_properties[name] = convert<std::string>(value);
    }

    bool hasValue(const std::string & name);

    std::string getAppName();

private:
    void parse(const std::string & filename);
    void parse(int argc, const char * argv[]);

    template<typename T, typename U> T convert(U value) const
    {
        T ret;
        std::stringstream ss;
        ss << value;
        ss >> ret;
        return ret;
    }

    bool exists(const std::string file);

    std::map<std::string, std::string> m_properties;
    std::string m_appName;
};

}
}

