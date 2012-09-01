#pragma once

#include <boost/optional.hpp>

namespace Common
{
namespace Game
{

class Player
{
public:
    void setId(int id);
    int getId();

private:
    boost::optional<int> m_id;
};

}
}

