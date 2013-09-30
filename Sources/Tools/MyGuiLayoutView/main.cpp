#include <cassert>
#include <memory>

#include "Cake/DependencyInjection/Registry.hpp"
#include "Cake/Configuration/Configuration.hpp"

#include "Client/Graphics/Graphics.hpp"
#include "Gui/Gui.hpp"
#include "Input/Input.hpp"

int main(int argc, const char ** argv)
{
    using namespace Client;
    using namespace Cake::DependencyInjection;

    std::shared_ptr<Cake::Configuration::Configuration> configuration(new Cake::Configuration::Configuration(argc, argv));
    forInterface<Cake::Configuration::Configuration>().use(configuration);

    Graphics::Graphics graphics;
    Gui::Gui gui(graphics.getRenderWindow(), graphics.getSceneManager());
    Input::Input input(graphics.getRenderWindow(), graphics.getSceneManager(), graphics.getCamera(), gui);

    assert(argc > 1);

    gui.loadLayout(argv[1]);

    while (!graphics.getRenderWindow().isClosed())
    {
        input.frameStarted();

        if (!graphics.frameStarted()) break;
    }
}
