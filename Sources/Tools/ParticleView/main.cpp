#include <cassert>

#include "Cake/DependencyInjection/Registry.hpp"
#include "Cake/Configuration/Configuration.hpp"
#include "Cake/Diagnostics/Logger.hpp"

#include "Client/Graphics/Graphics.hpp"
#include "Gui/Gui.hpp"
#include "Input/Input.hpp"

int main(int argc, const char ** argv)
{
    using namespace Client;
    using namespace Cake::DependencyInjection;

    boost::shared_ptr<Cake::Configuration::Configuration> configuration(new Cake::Configuration::Configuration(argc, argv));
    forInterface<Cake::Configuration::Configuration>().use(configuration);

    Graphics::Graphics graphics;
    Gui::Gui gui(graphics.getRenderWindow(), graphics.getSceneManager());
    Input::Input input(graphics.getRenderWindow(), graphics.getSceneManager(), graphics.getCamera(), gui);

    assert(argc > 1);

    auto & scene = graphics.getSceneManager();

    auto * psNode = scene.getRootSceneNode()->createChildSceneNode();
    psNode->setPosition(Ogre::Vector3(0, 0, -10000));

    auto * ps = scene.createParticleSystem("particle", argv[1]);
    psNode->attachObject(ps);

    while (!graphics.getRenderWindow().isClosed())
    {
        LOG_DEBUG << ps->getEmitting();
        #if 0
            scene.destroyAllParticleSystems();
            ps = scene.createParticleSystem("particle", argv[1]);
            psNode->attachObject(ps);
        }
        #endif

        input.frameStarted();
        if (!graphics.frameStarted()) break;
    }
}

