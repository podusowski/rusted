#pragma once

#include "Client/Input/IMouseListener.hpp"

namespace Client
{
namespace Input
{

class IInput
{
public:
    virtual void addMouseListener(IMouseListener &) = 0;
};

}
}
