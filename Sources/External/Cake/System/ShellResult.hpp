#pragma once

#include <string>

namespace Cake
{
namespace System
{

class ShellResult
{
public:
    ShellResult();
    ShellResult(int, const std::string &, const std::string &); 
    int getReturnCode();
    std::string getStdOut();
    std::string getStdErr();

private:
    int m_returnCode;
    std::string m_stdOut;
    std::string m_stdErr;
};

}
}
