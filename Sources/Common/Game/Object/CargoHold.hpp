#pragma once

namespace Common
{
namespace Game
{
namespace Object
{

class CargoHold
{
public:
    CargoHold();

    unsigned getHelium();
    void setHelium(unsigned);
    void changeHelium(int);

    unsigned getCarbon();
    void setCarbon(unsigned);
    void changeCarbon(int);

private:
    unsigned m_helium;
    unsigned m_carbon;
};

}
}
}

