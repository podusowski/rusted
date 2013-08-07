TARGET:=libCake.a
TYPE:=static_library

CFLAGS+=-I. -I.. -I$(TARGET_BASE) -I$(TARGET_BASE)/..

SOURCES+=Configuration/Configuration.cpp

SOURCES+=Diagnostics/Logger.cpp
SOURCES+=Diagnostics/LogMessage.cpp

SOURCES+=Threading/Mutex.cpp
SOURCES+=Threading/ConditionVariable.cpp
SOURCES+=Threading/ScopedLock.cpp
SOURCES+=Threading/Thread.cpp

SOURCES+=Networking/Detail/SocketDescriptor.cpp
SOURCES+=Networking/DnsResolver.cpp
SOURCES+=Networking/GenericPacket.cpp
SOURCES+=Networking/ServerSocket.cpp
SOURCES+=Networking/ServerSocketPool.cpp
SOURCES+=Networking/Socket.cpp

SOURCES+=Networking/Protocol/BinaryCoder.cpp
SOURCES+=Networking/Protocol/BinaryDecoder.cpp
SOURCES+=Networking/Protocol/FcDecoder.cpp
SOURCES+=Networking/Protocol/CharVectorReadBuffer.cpp
SOURCES+=Networking/Protocol/CharVectorWriteBuffer.cpp
SOURCES+=Networking/Protocol/CakeReadBuffer.cpp
SOURCES+=Networking/Protocol/CakeWriteBuffer.cpp

SOURCES+=Serialization/Tms.cpp

SOURCES+=System/AtomicSyscall.cpp
SOURCES+=System/ShellResult.cpp
SOURCES+=System/Shell.cpp
SOURCES+=System/Time.cpp

SOURCES+=Utils/StringList.cpp

