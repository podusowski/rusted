#pragma once

#include <OgreRenderWindow.h>

#include "MyGUI.h"
#include "MyGUI_OgrePlatform.h"

namespace Client
{
namespace Gui
{

class Gui
{
public:
    Gui(Ogre::RenderWindow &, Ogre::SceneManager &);
    ~Gui();

    void loadLayout(const std::string &);
    void unloadAllLayouts();

    MyGUI::Gui & operator*();
    MyGUI::Gui * operator->();
private:
    void initResources();
    void initRenderer();

    Ogre::RenderWindow & m_ogreRenderWindow;
    Ogre::SceneManager & m_ogreSceneManager;

    MyGUI::Gui m_myGui;
    MyGUI::OgrePlatform m_myGuiOgrePlatform;
    std::vector<MyGUI::VectorWidgetPtr> m_myGuiLoadedLayouts;
};

}
}
