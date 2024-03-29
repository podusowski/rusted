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

    void setCapacity(unsigned capacity);
    unsigned getCapacity() const;
    unsigned getUsedCapacity() const;

    unsigned getHelium() const;
    void setHelium(unsigned);
    void changeHelium(int);

    unsigned getCarbon() const;
    void setCarbon(unsigned);
    void changeCarbon(int);

private:
    unsigned m_capacity;
    unsigned m_helium;
    unsigned m_carbon;
};

}
}
}

