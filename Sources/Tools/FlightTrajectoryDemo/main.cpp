#include <cassert>

#include "Cake/DependencyInjection/Registry.hpp"
#include "Cake/Configuration/Configuration.hpp"

#include "Common/Game/Object/Ship.hpp"
#include "Common/Game/Object/FlightTrajectory.hpp"
#include "Common/Game/RustedTime.hpp"

#include "Client/Graphics/Graphics.hpp"
#include "Gui/Gui.hpp"
#include "Input/Input.hpp"

using namespace Client;
using namespace Cake::DependencyInjection;


struct Object
{
    Object(Graphics::Graphics & graphics, unsigned speed, Common::Game::Position destination) : m_graphics(&graphics)
    {
        ship = new Common::Game::Object::Ship();
        ship->setSpeed(100);
        ship->setCourse(destination);

        Ogre::SceneManager & scene = graphics.getSceneManager();
        entity = scene.createEntity("Cube.mesh");
        node = scene.getRootSceneNode()->createChildSceneNode();
        node->attachObject(entity);
        node->setScale(100.0, 100.0, 100.0);
    }

    void update()
    {
        Common::Game::Position position = ship->getPosition();
        auto orientation = ship->getOrientation();

        node->setPosition(position.getX(), position.getY(), position.getZ());
        node->setOrientation(m_graphics->toOgreQuaternion(orientation));

        // apply Blender coordinations patch
        node->roll(Ogre::Degree(90));
        node->pitch(Ogre::Degree(90));
    }

    Common::Game::Object::Ship * ship;
    Ogre::Entity * entity;
    Ogre::SceneNode * node;
    Graphics::Graphics * m_graphics;
};

int main(int argc, const char ** argv)
{
    boost::shared_ptr<Common::Game::IRustedTime> time(new Common::Game::RustedTime());
    forInterface<Common::Game::IRustedTime>().use(time);

    boost::shared_ptr<Cake::Configuration::Configuration> configuration(new Cake::Configuration::Configuration(argc, argv));
    forInterface<Cake::Configuration::Configuration>().use(configuration);

    forInterface<Common::Game::Object::IFlightTrajectory>()
        .useFactory<GenericFactory0<Common::Game::Object::IFlightTrajectory, Common::Game::Object::FlightTrajectory> >();

    Graphics::Graphics graphics;
    //graphics.getSceneManager().setSkyBox(true, "SkyBox1", 8000);

    Gui::Gui gui(graphics.getRenderWindow(), graphics.getSceneManager());
    Input::Input input(graphics.getRenderWindow(), graphics.getSceneManager(), graphics.getCamera(), gui);

    // create some light
    Ogre::Light* pointLight = graphics.getSceneManager().createLight("pointLight");
    pointLight->setType(Ogre::Light::LT_POINT);
    pointLight->setPosition(Ogre::Vector3(0, 150, 250));
    pointLight->setPowerScale(20.0);

    // create ships
    std::vector<Object> g_objects;
    for(int i = -10; i <= 10; i++)
        for(int j = -10; j <= 10; j++)
            for(int k = -10; k <= 10; k++)
                g_objects.push_back(Object(graphics, 100, Common::Game::Position(i * 1000, j * 1000, k * 1000)));

    graphics.getSceneManager().setSkyBox(true, "SkyBox1", 8000);

    unsigned cameraAngle = 0;

    while (!graphics.getRenderWindow().isClosed())
    {
        input.frameStarted();

        for (auto o: g_objects)
        {
            o.update();
        }

        cameraAngle += 1;
        Ogre::Quaternion q(Ogre::Degree(cameraAngle), Ogre::Vector3(0, 1, 0));
        Ogre::Vector3 v(0, 0, 10000);
        v = q * v;

        Ogre::Camera & camera = graphics.getCamera();
        camera.setPosition(v);
        camera.lookAt(0, 0, 0);

        if (cameraAngle % 200 == 0)
        {
            std::cerr << graphics.getRenderWindow().getAverageFPS() << "fps\n";
        }

        if (!graphics.frameStarted()) break;
    }
}
