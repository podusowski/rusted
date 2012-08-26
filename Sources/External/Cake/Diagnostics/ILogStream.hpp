#pragma once

namespace Cake
{
namespace Diagnostics
{

class ILogStream
{
public:
    virtual void flush(const std::string &) = 0;

    virtual ~ILogStream() {}
};

}
}
