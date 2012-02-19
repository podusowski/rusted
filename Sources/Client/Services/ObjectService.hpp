#include "Services/AbstractService.hpp"

namespace Client
{
namespace Services
{

class ObjectService : public AbstractService<ObjectService> 
{
public:
    void handle(const Common::Messages::VisibleObjects &);
    void handle(const Common::Messages::AbstractMessage &) {}
};

}
}
