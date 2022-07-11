include(ExternalProject)
set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_BINARY_DIR})
ExternalProject_Add(soci SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/soci-3.2.1 PREFIX ${CMAKE_CURRENT_BINARY_DIR} INSTALL_COMMAND "")
add_library(soci_core INTERFACE)
#FIXME: ugly, needs to be reworked
target_link_libraries(soci_core INTERFACE ${CMAKE_CURRENT_BINARY_DIR}/src/soci-build/lib/libsoci_core.a)
target_include_directories(soci_core INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/soci-3.2.1/core/)
