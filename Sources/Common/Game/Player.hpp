#pragma once

#include <boost/optional.hpp>

#include "IPlayer.hpp"
#include "Common/Game/Object/ObjectBase.hpp"

namespace Common
{
namespace Game
{

class Player : public IPlayer
{
public:
    Player();
    ~Player();

    void setId(int id);
    int getId();

    void setState(PlayerState);
    PlayerState getState();

    void selectObject(Common::Game::Object::ObjectBase &);
    Common::Game::Object::ObjectBase & getSelectedObject();
    Common::Game::Object::ObjectBase::Id getSelectedObjectId();

    void focusObject(Common::Game::Object::ObjectBase &);
    Common::Game::Object::ObjectBase & getFocusedObject();
    Common::Game::Object::ObjectBase::Id getFocusedObjectId();

    unsigned getHelium();
    void setHelium(unsigned);

    unsigned getCarbon();
    void setCarbon(unsigned);

    std::string getName();
    void setName(const std::string &);

private:
    boost::optional<int> m_id;
    PlayerState m_state;
    Common::Game::Object::ObjectBase * m_selectedObject;
    Common::Game::Object::ObjectBase * m_focusedObject;

    unsigned m_helium;
    unsigned m_carbon;
    std::string m_name;
};

}
}

