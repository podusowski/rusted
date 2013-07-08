#include <boost/shared_ptr.hpp>
#include <soci.h>

#include "Cake/DependencyInjection/Factory.hpp"
#include "Cake/DependencyInjection/Inject.hpp"
#include "Cake/Configuration/Configuration.hpp"

namespace Server
{
namespace DataBase
{

class SociSessionFactory : public Cake::DependencyInjection::Factory<soci::session>
{
public:
    boost::shared_ptr<soci::session> create();

private:
    Cake::DependencyInjection::Inject<Cake::Configuration::Configuration> m_cfg;
};

}
}

