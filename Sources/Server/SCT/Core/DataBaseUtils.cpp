#include <fstream>

#include "Common/Game/Utilities/PasswordHash.hpp"
#include "DataBaseUtils.hpp"

using namespace SCT;

std::string DataBaseUtils::createOnePlayerWithOneEntity()
{
    Common::Game::Utilities::PasswordHash hash;
    std::string filename = "/var/tmp/onePlayerWithOneEntity.xml";
    std::ofstream out(filename.c_str());
    out << 
    "<root>"
    "   <users>"
    "       <user id=\"1\" login=\"user1\" password=\"" + hash.generate("password") + "\" />"
    "   </users>"
    "   <entities>"
    "       <entity id=\"1\" player_id=\"1\" x=\"1\" y=\"1\" z=\"1\" />"
    "   </entities>"
    "   <staticObjects>"
    "       <staticObject id=\"1\" x=\"100\" y=\"100\" z=\"100\" />"
    "   </staticObjects>"
    "   <objects>"
    "       <object id=\"1\" type=\"Ship\" x=\"1\" y=\"1\" z=\"1\" owner=\"1\" />"
    "   </objects>"
    "</root>";

   return filename; 
}

