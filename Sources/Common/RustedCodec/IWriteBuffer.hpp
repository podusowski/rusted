#ifndef COMMON_RUSTEDCODEC_IWRITEBUFFER
#define COMMON_RUSTEDCODEC_IWRITEBUFFER

namespace Common
{

namespace RustedCodec
{

class IWriteBuffer
{
public:
	virtual void write(const void * source, size_t size) = 0;

	virtual ~IWriteBuffer() {}
};

}

}

#endif
