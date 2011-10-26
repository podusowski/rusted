#pragma once

#include <boost/optional.hpp>

namespace Client
{
namespace Game
{

class PlayerInfo
{
public:
    void setId(int id);
    int getId();

private:
    boost::optional<int> m_id;
};

}
}

