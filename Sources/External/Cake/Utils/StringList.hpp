#pragma once

#include <vector>
#include <string>

namespace Cake
{
namespace Utils
{

class StringList
{
public:
    void add(const std::string &);
    std::string toString() const;

private:
    std::vector<std::string> m_lines;
};

}
}

