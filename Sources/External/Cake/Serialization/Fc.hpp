#pragma once

#include <string>
#include <stdexcept>

namespace Cake
{
namespace Serialization
{

enum FcState
{
    FcState_WaitingForName,
    FcState_ReadingName,
    FcState_WaitingForOpeningBrace,
    FcState_WaitingForParameter,
    FcState_WaitingForMandatoryParameter,
    FcState_ReadingParameter,
    FcState_WaitingForColon,
    FcState_Finished
};

class Fc
{
public:
    Fc(const std::string & s)
    {
        FcState state = FcState_WaitingForName;
        std::string::const_iterator it = s.begin();
        std::string parameter;

        while (it != s.end())
        {
            if (state == FcState_WaitingForName)
            {
                if (*it != ' ')
                {
                    state = FcState_ReadingName;
                    continue;
                }
            }
            else if (state == FcState_ReadingName)
            {
                if (*it == '(')
                {
                    state = FcState_WaitingForParameter;
                }
                else if (*it == ' ')
                {
                    state = FcState_WaitingForOpeningBrace;
                }
                else
                {
                    m_name += *it;
                }
            }
            else if (state == FcState_WaitingForOpeningBrace)
            {
                if (*it == '(')
                {
                    state = FcState_WaitingForParameter;
                }
            }
            else if (state == FcState_WaitingForParameter)
            {
                if (*it == ')')
                {
                    state = FcState_Finished;
                }
                else if (*it == ',')
                {
                    unexpectedToken(*it);
                }
                else if (*it != ' ')
                {
                    state = FcState_ReadingParameter;
                    continue;
                }
            }
            else if (state == FcState_WaitingForMandatoryParameter)
            {
                if (*it == ')')
                {
                    unexpectedToken(*it);
                }
                else if (*it == ',')
                {
                    unexpectedToken(*it);
                }
                else if (*it == ' ')
                {
                }
                else
                {
                    state = FcState_ReadingParameter;
                    continue;
                }
            }
            else if (state == FcState_ReadingParameter)
            {
                if (*it == ',')
                {
                    m_parameters.push_back(parameter);
                    parameter = "";
                    state = FcState_WaitingForMandatoryParameter;
                }
                else if (*it == ')')
                {
                    m_parameters.push_back(parameter);
                    parameter = "";
                    state = FcState_Finished;
                }
                else if (*it == ' ')
                {
                    m_parameters.push_back(parameter);
                    parameter = "";
                    state = FcState_WaitingForColon;
                }
                else
                {
                    parameter += *it;
                }
            }
            else if (state == FcState_WaitingForColon)
            {
                if (*it == ',')
                {
                    state = FcState_WaitingForParameter;
                }
                else if (*it == ')')
                {
                    state = FcState_Finished;
                }
            }

            it++;
        }

        if (state != FcState_Finished)
        {
            throw std::runtime_error("unexpected end of string");
        }
    }

    std::string getName() const
    {
        return m_name;
    }

    std::vector<std::string> getParameters() const
    {
        return m_parameters;
    }

private:
    void unexpectedToken(char c) const
    {
        std::string s = "unexpected Fc token: ";
        s += c;
        throw std::runtime_error(s);
    }

    std::string m_name;
    std::vector<std::string> m_parameters;
};

}
}

