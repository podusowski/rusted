cmake_minimum_required(VERSION 3.18)

project(rusted LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror")

add_subdirectory(Sources/AdministrationClient)
add_subdirectory(Sources/External)
add_subdirectory(Sources/Common)