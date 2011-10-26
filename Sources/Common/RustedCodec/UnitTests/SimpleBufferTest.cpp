#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <vector>

#include <boost/assign.hpp>

#include <RustedCodec/SimpleWriteBuffer.hpp>
#include <RustedCodec/SimpleReadBuffer.hpp>

class SimpleBufferTest : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE (SimpleBufferTest);
	CPPUNIT_TEST (testSimpleWrite);
	CPPUNIT_TEST (testSimpleRead);
	CPPUNIT_TEST_SUITE_END ();

public:

protected:
	void testSimpleWrite();
	void testSimpleRead();
};

CPPUNIT_TEST_SUITE_REGISTRATION (SimpleBufferTest);

void SimpleBufferTest::testSimpleWrite()
{
	using ::Common::RustedCodec::SimpleWriteBuffer;

	const char * someData = "abcd";

	std::vector<char> rawBuffer;
	SimpleWriteBuffer buffer(rawBuffer);

	buffer.write(someData, 4);

	CPPUNIT_ASSERT(::memcmp(someData, &(*rawBuffer.begin()), 4) == 0);
}

void SimpleBufferTest::testSimpleRead()
{
	using namespace ::boost::assign;
	using ::Common::RustedCodec::SimpleReadBuffer;

	char * someData = new char[4];

	std::vector<char> rawBuffer;
	rawBuffer += 1, 2, 3, 4;

	SimpleReadBuffer buffer(rawBuffer);

	buffer.read(someData, 4);

	CPPUNIT_ASSERT(::memcmp(someData, &(*rawBuffer.begin()), 4) == 0);
}
