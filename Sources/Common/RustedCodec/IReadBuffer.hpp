#ifndef COMMON_RUSTEDCODEC_ABSTRACTREADBUFFER
#define COMMON_RUSTEDCODEC_ABSTRACTREADBUFFER

namespace Common
{

namespace RustedCodec
{

class IReadBuffer
{
public:
	virtual void read(void * destinationBuffer, size_t size) = 0;

	virtual ~IReadBuffer() {}
};

}

}

#endif
