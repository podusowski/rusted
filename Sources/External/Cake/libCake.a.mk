TARGET:=libCake
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
SOURCES+=Networking/Socket.cpp
SOURCES+=Networking/SocketStream.cpp

SOURCES+=System/AtomicSyscall.cpp
SOURCES+=System/ShellResult.cpp
SOURCES+=System/Shell.cpp
SOURCES+=System/Time.cpp
