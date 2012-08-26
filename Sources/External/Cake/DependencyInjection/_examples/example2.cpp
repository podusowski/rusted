#include <iostream>
#include "DependencyInjection/Inject.hpp"
#include "DependencyInjection/Registry.hpp"

class IUrlDownloader
{
public:
    virtual ~IUrlDownloader() {}
};

class WinApiUrlDownloader : public IUrlDownloader
{
public:
    WinApiUrlDownloader(const std::string & url) 
    {
        std::cout << CAKE_DEPENDENCY_INJECTION_TYPENAME(*this) << " is downloading " << url << "\n";
    }
};


class Application 
{
public:
    Application(const std::string & url) : m_downloader(url) {}

private:
    Cake::DependencyInjection::Inject<IUrlDownloader, const std::string &> m_downloader;
};



int main()
{
    Cake::DependencyInjection::forInterface<IUrlDownloader>().useFactory<
        Cake::DependencyInjection::GenericFactory1<IUrlDownloader, WinApiUrlDownloader, const std::string &> >();

    Application a("http://google.com");
}

