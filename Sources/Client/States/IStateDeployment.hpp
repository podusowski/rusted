#pragma once

namespace Client
{
namespace States
{

class IStateDeployment
{
public:
    virtual void deployNewConnection() = 0;
    virtual void deployAuthorizedConnection() = 0;

    virtual ~IStateDeployment() {}
};

}
}
