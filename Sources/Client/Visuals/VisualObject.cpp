#include <OgreSceneNode.h>
#include <OgreEntity.h>
#include <OgreParticleSystem.h>

#include "Cake/Diagnostics/Logger.hpp"

#include "Common/Game/Object/Asteroid.hpp"
#include "Common/Game/Object/Ship.hpp"
#include "VisualObject.hpp"

using namespace Client::Visuals;

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

    m_model = Cake::Serialization::Tms(f);

    std::string mesh = m_model.getValue<std::string>("mesh");
    m_entity = scene.createEntity(mesh);
    m_node = scene.getRootSceneNode()->createChildSceneNode();
    m_node->attachObject(m_entity);
    m_node->setScale(100.0, 100.0, 100.0);

    input.addEntityClickedCallback(*m_entity, std::bind(&VisualObject::entityClickedCallback, this));
    input.addEntityMouseMovedCallback(*m_entity, std::bind(&VisualObject::entityMouseMoved, this));
    input.addEntityMouseLeavedCallback(*m_entity, std::bind(&VisualObject::entityMouseLeaved, this));

    try
    {
        for (int i = 1; i < 10; i++)
        {
            std::stringstream engineNameSs;
            engineNameSs << "engine";
            engineNameSs << i;

            int x = m_model.getValue<int>(engineNameSs.str() + ".thrust.x");
            int y = m_model.getValue<int>(engineNameSs.str() + ".thrust.y");
            int z = m_model.getValue<int>(engineNameSs.str() + ".thrust.z");

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

    m_naturalMaterial = m_entity->getSubEntity(0)->getMaterialName();
    
    createLabel();
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

std::string VisualObject::getString(int type)
{
    std::stringstream ss;

    if (m_object.is<Common::Game::Object::Ship>())
    {
        auto & ship = dynamic_cast<Common::Game::Object::Ship&>(m_object);

        bool newLineNeeded = false;

        if (type & StringType_Class)
        {
            ss << m_model.getValue<std::string>("name") << " (" << ship.getId() << ")";
            newLineNeeded = true;
        }

        if (type & StringType_Pilot)
        {
            if (newLineNeeded)
            {
                ss << "\n";
            }

            ss << "Pilot: " << m_ownerName;
            newLineNeeded = true;
        }

        if (type & StringType_Integrity)
        {
            if (newLineNeeded)
            {
                ss << "\n";
            }

            ss << "Integrity: " << ship.getIntegrity();
            newLineNeeded = true;
        }

        if (type & StringType_CargoHold)
        {
            if (newLineNeeded)
            {
                ss << "\n";
            }

            ship.visitCargoHold([&](Common::Game::Object::CargoHold & cargoHold) -> void
            {
                ss << "C:" << cargoHold.getCarbon() << " H:" << cargoHold.getHelium() << " (" << cargoHold.getCapacity() << ")";
            });
            newLineNeeded = true;
        }
    }
    else if (m_object.is<Common::Game::Object::Asteroid>())
    {
        auto & asteroid = dynamic_cast<Common::Game::Object::Asteroid&>(m_object);
        ss << "Asteroid";
    }
    else
    {
        ss << "Object <" << m_object.getId() << ">";
    }

    return ss.str();
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
    updateWreckedState();
}

void VisualObject::entityClickedCallback()
{
    if (m_rightClickCallback)
        m_rightClickCallback();
}

void VisualObject::entityMouseMoved()
{
    m_label->setVisible(true);
}

void VisualObject::entityMouseLeaved()
{
    m_label->setVisible(false);
}

void VisualObject::setEngineThrustEnabled(bool v)
{
    for (auto ps: m_engineThrustParticleSystems)
    {
        ps->setEmitting(v);
    }
}

void VisualObject::createLabel()
{
    m_label = m_gui->createWidget<MyGUI::TextBox>("TextBox", MyGUI::IntCoord(10, 10, 150, 50), MyGUI::Align::Default, "Main");
    m_label->setVisible(false);

    updateLabelText();

    if (m_object.is<Common::Game::Object::Ship>())
    {
        auto & ship = dynamic_cast<Common::Game::Object::Ship&>(m_object);

        m_objectService.fetchPlayerName(ship.getOwnerId(),
            std::bind(&VisualObject::ownerNameFetched, this, std::placeholders::_1, std::placeholders::_2));
    }
}

void VisualObject::updateLabelText()
{
    m_label->setCaption(getString());
}

void VisualObject::updateLabel()
{
    auto screenCoords = m_graphics.getScreenCoordinates(*m_entity);
    if (std::get<0>(screenCoords))
    {
        auto position = std::get<1>(screenCoords);
        m_label->setPosition(MyGUI::IntPoint(position.x, position.y - 20));
    }
    //m_label->setVisible(std::get<0>(screenCoords));
}

void VisualObject::ownerNameFetched(unsigned id, const std::string & name)
{
    m_ownerName = name;
    updateLabelText();
}

void VisualObject::updateWreckedState()
{
    if (m_object.is<Common::Game::Object::Ship>())
    {
        auto & ship = dynamic_cast<Common::Game::Object::Ship&>(m_object);
        std::string currentMaterial = m_entity->getSubEntity(0)->getMaterialName();
        if (currentMaterial != m_naturalMaterial && !ship.isWrecked())
        {
            LOG_DEBUG << "Switching to normal material: " << m_naturalMaterial;
            m_entity->getSubEntity(0)->setMaterialName(m_naturalMaterial);
        }
        else if (currentMaterial == m_naturalMaterial && ship.isWrecked())
        {
            LOG_DEBUG << "Switching to wrecked material";
            m_entity->getSubEntity(0)->setMaterialName("Wrecked");
        }
    }
}

