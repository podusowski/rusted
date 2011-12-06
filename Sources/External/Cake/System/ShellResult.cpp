#include "System/ShellResult.hpp"

using namespace Cake::System;

ShellResult::ShellResult() :
    m_returnCode(0)
{
}

ShellResult::ShellResult(int returnCode, const std::string & stdOut, const std::string & stdErr) :
    m_returnCode(returnCode),
    m_stdOut(stdOut),
    m_stdErr(stdErr)
{
}

int ShellResult::getReturnCode()
{
    return m_returnCode;
}

std::string ShellResult::getStdOut()
{
    return m_stdOut;
}

std::string ShellResult::getStdErr()
{
    return m_stdErr;
}
