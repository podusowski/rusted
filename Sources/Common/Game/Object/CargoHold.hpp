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

    unsigned getCarbon();
    void setCarbon(unsigned);

private:
    unsigned m_helium;
    unsigned m_carbon;
};

}
}
}

