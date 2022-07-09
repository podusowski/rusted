add_library(hashlibpp
    hl_md5.cpp hl_md5wrapper.cpp hl_sha1.cpp hl_sha1wrapper.cpp
    hl_sha256.cpp hl_sha256wrapper.cpp hl_sha2ext.cpp hl_sha384wrapper.cpp
    hl_sha512wrapper.cpp hl_wrapperfactory.cpp
)

target_include_directories(hashlibpp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})