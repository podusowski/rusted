#pragma once

#include <OgreRenderWindow.h>
#include "Gui/CEGUIIncludes.hpp"

namespace Client
{
namespace Gui
{

class Gui
{
public:
    Gui(Ogre::RenderWindow & ogreRenderWindow);
    CEGUI::WindowManager & getCeguiWindowManager();
    void setLayout(const std::string &);
    CEGUI::Window & getLayoutWindow();
private:
    void initResources();
    void initRenderer();

    Ogre::RenderWindow & m_ogreRenderWindow;
    CEGUI::Window * m_layoutWindow;
};

}
}
