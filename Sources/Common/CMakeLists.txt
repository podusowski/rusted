set(SRC
Game/RustedTime.cpp
Game/TimeValue.cpp
Game/Utilities/PasswordHash.cpp
Game/Utilities/IdGenerator.cpp
Game/Universe.cpp
Game/Player.cpp
Game/Object/ObjectBase.cpp
Game/Object/FlightTrajectory.cpp
Game/Object/Ship.cpp
Game/Object/Asteroid.cpp
Game/Object/CargoHold.cpp
Math/Quaternion.cpp
Math/Bezier3.cpp
Math/Matrix3.cpp
Math/KinematicParticle.cpp
Messages/Messages.hpp #generated
)

add_library(common ${SRC})
target_include_directories(common SYSTEM PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/..) #FIXME
target_link_libraries(common PUBLIC cake)
target_link_libraries(common PRIVATE hashlibpp)

# FIXME: generate that outside of src dir, possibly using protobuf compiler directly?
add_custom_command(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/Messages/Messages.hpp COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/../External/Cake/Networking/Protocol/protoc.py --namespace Common::Messages --output ${CMAKE_CURRENT_SOURCE_DIR}/Messages/Messages.hpp ${CMAKE_CURRENT_SOURCE_DIR}/Messages/Messages.xml)