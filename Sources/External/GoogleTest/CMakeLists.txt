add_library(gtest gmock-gtest-all.cpp)
target_include_directories(gtest PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})