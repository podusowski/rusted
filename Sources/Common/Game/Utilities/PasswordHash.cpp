#include <hashlibpp.h>

#include "Game/Utilities/PasswordHash.hpp"

using namespace Common::Game::Utilities;

std::string PasswordHash::generate(std::string password)
{
    sha1wrapper hash;
    return hash.getHashFromString(password);
}

