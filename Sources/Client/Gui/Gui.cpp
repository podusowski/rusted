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
    }
    catch (const std::exception & ex)
    {
        LOG_ERR << "Can't initialize GUI, reason: " << ex.what();
        throw;
    }
}

Gui::~Gui()
{
    LOG_INFO << "Shutting down GUI subsystem";
    m_myGui->shutdown();
    m_myGuiOgrePlatform->shutdown();
}

void Gui::loadLayout(const std::string & layout)
{
    LOG_DEBUG << "Loading GUI layout: " << layout;

    MyGUI::LayerManager::getInstancePtr()->resizeView(MyGUI::IntSize(800, 600));

    unloadAllLayouts();
    auto l = MyGUI::LayoutManager::getInstance().loadLayout(layout);
    m_myGuiLoadedLayouts.push_back(l);

    MyGUI::LayerManager::getInstancePtr()->resizeView(MyGUI::IntSize(m_ogreRenderWindow.getWidth(), m_ogreRenderWindow.getHeight()));
}

void Gui::unloadAllLayouts()
{
    LOG_DEBUG << "Unloading all layouts";

    for (auto layout: m_myGuiLoadedLayouts)
    {
        MyGUI::LayoutManager::getInstance().unloadLayout(layout);
    }
    m_myGuiLoadedLayouts.clear();
}

MyGUI::Gui & Gui::operator*()
{
    return *m_myGui;
}

MyGUI::Gui * Gui::operator->()
{
    return m_myGui.get();
}

void Gui::initResources()
{
    LOG_DEBUG << "Initializing GUI resources";
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
    LOG_DEBUG << "Initializing GUI renderer";
    m_myGuiOgrePlatform = std::make_shared<MyGUI::OgrePlatform>();
    m_myGuiOgrePlatform->initialise(&m_ogreRenderWindow, &m_ogreSceneManager);

    m_myGui = std::make_shared<MyGUI::Gui>();
    m_myGui->initialise();
}

