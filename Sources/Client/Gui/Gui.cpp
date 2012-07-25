#include "Cake/Diagnostics/Logger.hpp"
#include "Gui/Gui.hpp"

using namespace Client::Gui;

Gui::Gui(Ogre::RenderWindow & ogreRenderWindow, Ogre::SceneManager & ogreSceneManager) : 
    m_ogreRenderWindow(ogreRenderWindow),
    m_ogreSceneManager(ogreSceneManager)
{
    LOG_INFO << "Initializing GUI subsystem";

    initResources();
    initRenderer();
}

void Gui::loadLayout(const std::string & layout)
{
    LOG_DEBUG << "Loading GUI layout: " << layout;

    MyGUI::LayoutManager::getInstance().unloadLayout(m_myGuiLoadedLayout);
    m_myGuiLoadedLayout = MyGUI::LayoutManager::getInstance().loadLayout(layout);
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
    m_myGuiOgrePlatform.initialise(&m_ogreRenderWindow, &m_ogreSceneManager);
    m_myGui.initialise();
}

