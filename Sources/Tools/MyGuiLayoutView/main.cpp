#include <cassert>

#include "Client/Graphics/Graphics.hpp"
#include "Gui/Gui.hpp"
#include "Input/Input.hpp"

int main(int argc, char * argv[])
{
    using namespace Client;

    Graphics::Graphics graphics;
    Gui::Gui gui(graphics.getRenderWindow(), graphics.getSceneManager());
    Input::Input input(graphics.getRenderWindow(), graphics.getSceneManager(), graphics.getCamera(), gui);

    assert(argc > 1);

    gui.loadLayout(argv[1]);

    while (!graphics.getRenderWindow().isClosed())
    {
        input.frameStarted();

        graphics.getRenderWindow().swapBuffers(true);
        Ogre::WindowEventUtilities::messagePump();

        if (!graphics.getOgreRoot().renderOneFrame()) break;
    }
}
