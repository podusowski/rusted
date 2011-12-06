LIBNAME=libCake.a

CFLAGS+=-I.

SOURCES+=Configuration/Configuration.cpp

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

include Makefile.leaf
