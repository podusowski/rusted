add_library(Cake
    Configuration/Configuration.cpp

    Diagnostics/Logger.cpp
    Diagnostics/LogMessage.cpp

    Threading/Mutex.cpp
    Threading/ConditionVariable.cpp
    Threading/ScopedLock.cpp
    Threading/Thread.cpp

    Networking/SocketInitialize.cpp
    Networking/Detail/SocketDescriptor.cpp
    Networking/Detail/Error.cpp
    Networking/DnsResolver.cpp
    Networking/GenericPacket.cpp
    Networking/Acceptor.cpp
    Networking/MainLoop.cpp
    Networking/Socket.cpp

    Networking/Protocol/BinaryCoder.cpp
    Networking/Protocol/BinaryDecoder.cpp
    Networking/Protocol/FcDecoder.cpp
    Networking/Protocol/CharVectorReadBuffer.cpp
    Networking/Protocol/CharVectorWriteBuffer.cpp
    Networking/Protocol/CakeReadBuffer.cpp
    Networking/Protocol/CakeWriteBuffer.cpp

    Serialization/Tms.cpp

    System/AtomicSyscall.cpp
    System/ShellResult.cpp
    System/Shell.cpp
    System/Time.cpp

    Utils/StringList.cpp
)

target_include_directories(Cake PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/..) # FIXME
find_package(Threads REQUIRED)
target_link_libraries(Cake PUBLIC boost Threads::Threads)