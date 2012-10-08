#pragma once

#include "IEffect.hpp"

namespace Client
{
namespace Graphics
{

class MovingMeshEffect : public IEffect
{
public:
    void frameStarted(); 
    bool isAlive();
};

}
}

