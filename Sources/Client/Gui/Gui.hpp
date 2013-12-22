#pragma once

#include <boost/utility.hpp>
#include <memory>

#include <OgreRenderWindow.h>

#include "MyGUI.h"
#include "MyGUI_OgrePlatform.h"

namespace Client
{
namespace Gui
{

class Gui : boost::noncopyable
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

    std::shared_ptr<MyGUI::OgrePlatform> m_myGuiOgrePlatform;
    std::shared_ptr<MyGUI::Gui> m_myGui;
    std::vector<MyGUI::VectorWidgetPtr> m_myGuiLoadedLayouts;
};

}
}
