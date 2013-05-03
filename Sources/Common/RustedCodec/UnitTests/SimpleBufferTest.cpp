#include <gtest/gtest.h>

#include <vector>

#include "RustedCodec/SimpleWriteBuffer.hpp"
#include "RustedCodec/SimpleReadBuffer.hpp"

TEST(SimpleBufferTest, SimpleWrite)
{
	using ::Common::RustedCodec::SimpleWriteBuffer;

	const char * someData = "abcd";

	std::vector<char> rawBuffer;
	SimpleWriteBuffer buffer(rawBuffer);

	buffer.write(someData, 4);

	EXPECT_TRUE(::memcmp(someData, &(*rawBuffer.begin()), 4) == 0);
}

TEST(SimpleBufferTest, SimpleRead)
{
	using ::Common::RustedCodec::SimpleReadBuffer;

	char * someData = new char[4];
	std::vector<char> rawBuffer{1, 2, 3, 4};

	SimpleReadBuffer buffer(rawBuffer);

	buffer.read(someData, 4);

	EXPECT_TRUE(::memcmp(someData, &(*rawBuffer.begin()), 4) == 0);
}

