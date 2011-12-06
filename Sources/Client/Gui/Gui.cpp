#include "Common/Logger/Logger.hpp"
#include "Gui/Gui.hpp"

using namespace Client::Gui;

Gui::Gui(Ogre::RenderWindow & ogreRenderWindow) : m_ogreRenderWindow(ogreRenderWindow)
{
    LOG_INFO << "Initializing GUI subsystem\n";
    try
    {
        initRenderer();
        initResources();
        CEGUI::SchemeManager::getSingleton().create("TaharezLook.scheme");
        CEGUI::System::getSingleton().setDefaultMouseCursor((CEGUI::utf8*)"TaharezLook", (CEGUI::utf8*)"MouseArrow");
    }
    catch (CEGUI::Exception & ex)
    {
        LOG_ERR << "CEGUI dropped an exception of type " << TYPENAME(ex) << ", reason: " << ex.getMessage() << "\n";
    }
}

CEGUI::WindowManager & Gui::getCeguiWindowManager()
{
    return CEGUI::WindowManager::getSingleton();
}

void Gui::setLayout(const std::string & layout)
{
    m_layoutWindow = getCeguiWindowManager().loadWindowLayout(layout);
    CEGUI::System::getSingleton().setGUISheet(m_layoutWindow);
    m_layoutWindow->activate();
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

void Gui::initResources()
{
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation("Contents/imagesets", "FileSystem", "General");
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation("Contents/schemes", "FileSystem", "General");
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation("Contents/fonts", "FileSystem", "General");
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation("Contents/layouts", "FileSystem", "General");
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation("Contents/looknfeel", "FileSystem", "General");
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation("Contents/xml_schemas", "FileSystem", "General");
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

void Gui::initRenderer()
{
    CEGUI::OgreRenderer::bootstrapSystem(m_ogreRenderWindow);
    CEGUI::Logger::getSingleton().setLoggingLevel(CEGUI::Insane);
}



