#include <memory>
#include <soci.h>

#include "Cake/DependencyInjection/AbstractFactory.hpp"
#include "Cake/DependencyInjection/Inject.hpp"
#include "Cake/Configuration/Configuration.hpp"

namespace Server
{
namespace DataBase
{

class SociSessionFactory : public Cake::DependencyInjection::AbstractFactory<soci::session>
{
public:
    std::shared_ptr<soci::session> create();

private:
    Cake::DependencyInjection::Inject<Cake::Configuration::Configuration> m_cfg;
};

}
}

