add_library(pake STATIC
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