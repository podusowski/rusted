#pragma once

#include "IEffect.hpp"

namespace Client
{
namespace Graphics
{

class MovingBillboardEffect : public IEffect
{
public:
    void frameStarted(); 
    bool isAlive();
};

}
}

