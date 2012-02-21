#include "Common/Game/Universe.hpp"
#include "Services/AbstractService.hpp"

namespace Client
{
namespace Services
{

class ObjectService : public AbstractService<ObjectService> 
{
public:
    ObjectService(Common::Game::Universe &);

    void handle(const Common::Messages::VisibleObjects &);
    void handle(const Common::Messages::ShipInfo &);
    void handle(const Common::Messages::AbstractMessage &) {}

private:
    Common::Game::Universe & m_universe;
};

}
}
