#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <cstdio>
#include <boost/asio.hpp>

#include "Cake/Threading/Thread.hpp"

#include <RustedCodec/AsioReadBuffer.hpp>
#include <RustedCodec/AsioWriteBuffer.hpp>

class AsioBufferTest : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE (AsioBufferTest);
	CPPUNIT_TEST (testAsioReadBuffer);
	CPPUNIT_TEST (testAsioWriteBuffer);
	CPPUNIT_TEST_SUITE_END ();

public:

protected:
	void testAsioReadBuffer();
	void testAsioWriteBuffer();
};

//CPPUNIT_TEST_SUITE_REGISTRATION (AsioBufferTest);

void AsioBufferTest::testAsioReadBuffer()
{
	using boost::asio::ip::tcp;

	boost::asio::io_service io_service;

	// set up netcat to listen to connection and send "abcd" ascii string
	::system("bash -c \"echo -n abcd | netcat -l 1221 &\"");

	// we must wait a bit to get netcat up
	Cake::Threading::Thread::wait(0, 500);

	// connect to netcat
	tcp::endpoint addr(boost::asio::ip::address_v4::from_string("127.0.0.1"), 1221);
	tcp::socket socket(io_service);
	socket.connect(addr);

	::Common::RustedCodec::AsioReadBuffer buffer(socket);

	char a = 'x';
	buffer.read(&a, sizeof(a));

	CPPUNIT_ASSERT_EQUAL('a', a);

	// cleanup
	::system("pkill netcat");
}

void AsioBufferTest::testAsioWriteBuffer()
{
	using boost::asio::ip::tcp;

	boost::asio::io_service io_service;

	// set up netcat to listen to connection and save result to tmp file
	::system("bash -c \"netcat -l 1222 > /tmp/testAsioWriteBuffer.tmp &\"");

	// we must wait a bit to get netcat up
	Cake::Threading::Thread::wait(0, 500);

	// connect to netcat
	tcp::endpoint addr(boost::asio::ip::address_v4::from_string("127.0.0.1"), 1222);
	tcp::socket socket(io_service);
	socket.connect(addr);

	::Common::RustedCodec::AsioWriteBuffer buffer(socket);

	char a = 'a';
	buffer.write(&a, sizeof(a));

	char b = 'x';
	std::fstream tmpFile("/tmp/testAsioWriteBuffer.tmp");
	tmpFile.read(&b, sizeof(b));

	CPPUNIT_ASSERT_EQUAL(a, b);
}

