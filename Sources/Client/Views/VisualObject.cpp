#include <OgreSceneNode.h>
#include <OgreEntity.h>
#include <OgreParticleSystem.h>

#include "Cake/Diagnostics/Logger.hpp"

#include "Common/Game/Object/Asteroid.hpp"
#include "Common/Game/Object/Ship.hpp"
#include "VisualObject.hpp"

using namespace Client::Views;

VisualObject::VisualObject(
    Client::Graphics::IGraphics & graphics,
    Gui::Gui & gui,
    Client::Input::IInput & input,
    Common::Game::Object::ObjectBase & object,
    Client::Services::ObjectService & objectService)
    :
    m_graphics(graphics),
    m_gui(gui),
    m_object(object),
    m_objectService(objectService)
{
    Ogre::SceneManager & scene = m_graphics.getSceneManager();

    std::string modelFilename = "Contents/objects/" + object.getModel();

    LOG_DEBUG << "Loading model: " << modelFilename;

    std::ifstream f(modelFilename);

    if (!f.good())
    {
        std::stringstream ss;
        ss << "can't read model file: " << modelFilename;
        throw std::runtime_error(ss.str());
    }

    Cake::Serialization::Tms tms(f);

    std::string mesh = tms.getValue<std::string>("mesh");
    m_entity = scene.createEntity(mesh);
    m_node = scene.getRootSceneNode()->createChildSceneNode();
    m_node->attachObject(m_entity);
    m_node->setScale(100.0, 100.0, 100.0);

    input.addObjectRightClickCallback(*m_entity, std::bind(&VisualObject::rightClickedCallback, this));

    try
    {
        for (int i = 1; i < 10; i++)
        {
            std::stringstream engineNameSs;
            engineNameSs << "engine";
            engineNameSs << i;

            int x = tms.getValue<int>(engineNameSs.str() + ".thrust.x");
            int y = tms.getValue<int>(engineNameSs.str() + ".thrust.y");
            int z = tms.getValue<int>(engineNameSs.str() + ".thrust.z");

            LOG_DEBUG << "Engine" << i << " thrust at: " << x << ", " << y << ", " << z;

            std::stringstream ss;
            ss << "engine-particle-" << object.getId() << "-" << i;
            Ogre::ParticleSystem * ps = scene.createParticleSystem(ss.str(), "EngineTail");

            auto * psNode = m_node->createChildSceneNode();
            psNode->setPosition(Ogre::Vector3(x, y, z));
            psNode->attachObject(ps);

            m_engineThrustParticleSystems.push_back(ps);
        }
    }
    catch (const std::exception & ex)
    {
        LOG_ERR << "Can't create engine thrust effect, reason: " << ex.what();
    }
    
    createLabel(tms);
    update();
}

void VisualObject::setRightClickCallback(std::function<void()> callback)
{
    m_rightClickCallback = callback;
}

void VisualObject::setSelected(bool selected)
{
    m_node->showBoundingBox(selected);
}

Common::Game::Object::ObjectBase & VisualObject::getGameObject()
{
    return m_object;
}

void VisualObject::update()
{
    Common::Game::Position position = m_object.getPosition();
    auto orientation = m_object.getOrientation();

    m_node->setPosition(position.getX(), position.getY(), position.getZ());
    m_node->setOrientation(m_graphics.toOgreQuaternion(orientation));

    // apply Blender coordinations patch
    //m_node->roll(Ogre::Degree(90));
    //m_node->pitch(Ogre::Degree(90));
    m_node->yaw(Ogre::Degree(-90));

    setEngineThrustEnabled(m_object.isMoving());
    updateLabel();
}

void VisualObject::rightClickedCallback()
{
    if (m_rightClickCallback)
        m_rightClickCallback();
}

void VisualObject::setEngineThrustEnabled(bool v)
{
    for (auto ps: m_engineThrustParticleSystems)
    {
        ps->setEmitting(v);
    }
}

void VisualObject::createLabel(const Cake::Serialization::Tms & model)
{
    m_label = m_gui->createWidget<MyGUI::TextBox>("TextBox", MyGUI::IntCoord(10, 10, 50, 50), MyGUI::Align::Default, "Main");

    if (m_object.is<Common::Game::Object::Asteroid>())
    {
        m_label->setCaption("asteroid");
    }
    else if (m_object.is<Common::Game::Object::Ship>())
    {
        auto & ship = dynamic_cast<Common::Game::Object::Ship&>(m_object);

        std::stringstream ss;
        ss << model.getValue<std::string>("name") << "\n" << ship.getOwnerId();
        m_label->setCaption(ss.str());
    }
}

void VisualObject::updateLabel()
{
    auto screenCoords = m_graphics.getScreenCoordinates(*m_entity);
    if (std::get<0>(screenCoords))
    {
        auto position = std::get<1>(screenCoords);
        m_label->setPosition(MyGUI::IntPoint(position.x, position.y - 20));
    }
    m_label->setVisible(std::get<0>(screenCoords));
}

