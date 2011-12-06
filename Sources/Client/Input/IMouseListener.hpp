#pragma once

namespace Client
{
namespace Input
{

class IMouseListener
{
public:
    virtual void mouseMoved(const OIS::MouseState &) = 0;
    virtual void mousePressed(const OIS::MouseButtonID &, unsigned x, unsigned y) = 0;
    virtual void mouseReleased(const OIS::MouseButtonID &, unsigned x, unsigned y) = 0;

    virtual ~IMouseListener() {}
};

}
}
