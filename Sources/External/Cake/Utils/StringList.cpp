#include <sstream>

#include "StringList.hpp"

using namespace Cake::Utils;

StringList::NewLineStream StringList::add()
{
    return StringList::NewLineStream(*this);
}

void StringList::add(const std::string & line)
{
    m_lines.push_back(line);
}

std::string StringList::toString() const
{
    std::stringstream ret;
    bool needNewLine = false;

    for (const auto & line: m_lines)
    {
        if (needNewLine)
        {
            ret << "\n";
        }

        ret << line;

        needNewLine = true;
    }
    return ret.str();
}

unsigned StringList::size() const
{
    return m_lines.size();
}
