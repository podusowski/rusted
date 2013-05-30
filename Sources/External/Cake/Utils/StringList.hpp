#pragma once

#include <vector>
#include <string>
#include <sstream>

namespace Cake
{
namespace Utils
{

class StringList
{
public:

    class NewLineStream
    {
    public:
        NewLineStream(StringList & stringList) : m_stringList(stringList)
        {
        }

        NewLineStream(const NewLineStream & other) : m_stringList(other.m_stringList)
        {
        }

        ~NewLineStream()
        {
            m_stringList.add(m_ss.str());
        }

        template<class T> NewLineStream & operator<<(const T & value)
        {
            m_ss << value;
            return *this;
        }

    private:
        StringList & m_stringList;
        std::stringstream m_ss;
    };

    NewLineStream add();
    void add(const std::string &);
    std::string toString() const;
    unsigned size() const;

private:
    std::vector<std::string> m_lines;
};

}
}

