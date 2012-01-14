#include <cxxabi.h>
#include <iomanip>

#include "Cake/Threading/Thread.hpp"
#include "Logger.hpp"

using namespace Cake::Diagnostics;

Logger::Logger()
{
    m_banners.insert(std::make_pair(DEBUG,   "\033[1;37mDBG\033[0m"));
    m_banners.insert(std::make_pair(INFO,    "\033[1;34mINF\033[0m"));
    m_banners.insert(std::make_pair(WARNING, "\033[1;33mWRN\033[0m"));
    m_banners.insert(std::make_pair(ERROR,   "\033[1;31mERR\033[0m"));
}

Logger & Logger::getInstance()
{
    static Logger logger;
    return logger;
}

void Logger::setAppName(const std::string & appname)
{
    m_appName = appname;
}

std::string Logger::demangle(const std::string & name)
{
    int status;
    char * realname = abi::__cxa_demangle(name.c_str(), 0, 0, &status);
    std::string result(realname);
    free(realname);

    return result;
}

LogMessage Logger::log(LogLevel level, 
                       const std::string & file, 
                       unsigned line)
{
    return LogMessage(generateHeader(level, file, line));
}

std::string Logger::generateHeader(LogLevel level, const std::string & file, unsigned line)
{
    std::string file2 = file;
    const unsigned max_file_length = 20;

    if (file.length() > max_file_length)
    {
        file2 = ".." + file.substr(file.length() - max_file_length + 2, max_file_length - 2);
    }

    std::stringstream fileWithLine;
    fileWithLine << file2 << ":" << line;

    std::stringstream ss;
    ss  
        << std::setw(10) << std::setiosflags(std::ios::right) << m_appName << "("
        << "t:" << Cake::Threading::Thread::self() << ") "
        << std::setw(max_file_length + 4) << std::setiosflags(std::ios::right) << fileWithLine.str() 
        << " "
        << m_banners[level] << " ";
    return ss.str();
}

