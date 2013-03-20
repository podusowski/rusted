#include <sstream>
#include <istream>
#include <map>
#include <stdexcept>

namespace Cake
{
namespace Serialization
{

class Tms
{
public:
    Tms(std::istream & stream);

    template<typename T> T getValue(const std::string & name) const
    {
        auto it = m_data.find(name);
        if (it != m_data.end())
        {
            std::stringstream ss(it->second);
            T val;
            ss >> val;
            return val;
        }

        std::stringstream ss;
        ss << "no such parameter: " << name;
        throw std::runtime_error(ss.str());
    }

private:
    std::map<std::string, std::string> m_data;
};

}
}

