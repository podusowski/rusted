#include "Cake/Diagnostics/Logger.hpp"
#include "Gui/Gui.hpp"

using namespace Client::Gui;

Gui::Gui(Ogre::RenderWindow & ogreRenderWindow, Ogre::SceneManager & ogreSceneManager) : 
    m_ogreRenderWindow(ogreRenderWindow),
    m_ogreSceneManager(ogreSceneManager)
{
    LOG_INFO << "Initializing GUI subsystem";
    try
    {
        initResources();
        initRenderer();
        CEGUI::SchemeManager::getSingleton().create("TaharezLook.scheme");
        CEGUI::System::getSingleton().setDefaultMouseCursor((CEGUI::utf8*)"TaharezLook", (CEGUI::utf8*)"MouseArrow");
    }
    catch (CEGUI::Exception & ex)
    {
        LOG_ERR << "CEGUI dropped an exception of type " << TYPENAME(ex) << ", reason: " << ex.getMessage();
    }
}

CEGUI::WindowManager & Gui::getCeguiWindowManager()
{
    return CEGUI::WindowManager::getSingleton();
}

void Gui::setLayout(const std::string & layout)
{
    MyGUI::LayoutManager::getInstance().loadLayout(layout);
}

CEGUI::Window & Gui::getLayoutWindow()
{
    if (m_layoutWindow)
    {
        return *m_layoutWindow;
    }
    else
    {
        throw std::exception();
    }
}

MyGUI::Gui & Gui::operator*()
{
    return m_myGui;
}

MyGUI::Gui * Gui::operator->()
{
    return &m_myGui;
}

void Gui::initResources()
{
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation("Contents/imagesets", "FileSystem", "General");
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation("Contents/schemes", "FileSystem", "General");
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation("Contents/fonts", "FileSystem", "General");
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation("Contents/layouts", "FileSystem", "General");
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation("Contents/looknfeel", "FileSystem", "General");
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation("Contents/xml_schemas", "FileSystem", "General");
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation("Contents/gui", "FileSystem", "General");
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

void Gui::initRenderer()
{
    CEGUI::OgreRenderer::bootstrapSystem(m_ogreRenderWindow);
    CEGUI::Logger::getSingleton().setLoggingLevel(CEGUI::Insane);

    m_myGuiOgrePlatform.initialise(&m_ogreRenderWindow, &m_ogreSceneManager);
    m_myGui.initialise();
}

