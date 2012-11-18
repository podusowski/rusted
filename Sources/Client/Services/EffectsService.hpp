#pragma once

#include "Services/AbstractService.hpp"

namespace Client
{
namespace Services
{

class EffectsService : public AbstractService<EffectsService>
{
public:
    void handle(const Common::Messages::AbstractMessage &) {}
};

}
}
