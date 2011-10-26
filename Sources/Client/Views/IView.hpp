#pragma once

namespace Client
{
namespace Views
{

class IView
{
public:
    virtual void activate() = 0;
    virtual void deactivate() = 0;
    virtual void frameStarted() = 0;

    virtual ~IView() {}
};

}
}
