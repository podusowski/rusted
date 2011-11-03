#include <iostream>
#include "DependencyInjection/Inject.hpp"
#include "DependencyInjection/Registry.hpp"


// interface and some implementation

struct IService
{
    virtual ~IService() {}
};

struct ConcreteService : public IService 
{
};


// client class

class Application 
{
public:
    Application()
    {
        assert(typeid(*m_service) == typeid(ConcreteService));
        std::cout << "I'm using " << CAKE_DEPENDENCY_INJECTION_TYPENAME(*m_service) << "\n";
    }
private:
    Cake::DependencyInjection::Inject<IService> m_service;
};


// registry and deployment

int main()
{
    Cake::DependencyInjection::forInterface<IService>()
          .use<ConcreteService>();

    Application a;
}

