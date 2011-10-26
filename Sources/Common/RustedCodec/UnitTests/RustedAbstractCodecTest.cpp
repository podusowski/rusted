#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <string>

#include <RustedCodec/RustedAbstractCoder.hpp>
#include <RustedCodec/RustedAbstractDecoder.hpp>

#include <RustedCodec/SimpleWriteBuffer.hpp>
#include <RustedCodec/SimpleReadBuffer.hpp>

class RustedAbstractCodecTest : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE (RustedAbstractCodecTest);
	CPPUNIT_TEST (testSimpleStaticTypes);
	CPPUNIT_TEST (testSimpleStaticAndStringTypes);
	CPPUNIT_TEST (testListTypes);
	CPPUNIT_TEST (testListTypesWithDynamicData);
	CPPUNIT_TEST_SUITE_END ();

public:

protected:
	void testSimpleStaticTypes();
	void testSimpleStaticAndStringTypes();
	void testListTypes();
	void testListTypesWithDynamicData();
};

CPPUNIT_TEST_SUITE_REGISTRATION (RustedAbstractCodecTest);

void RustedAbstractCodecTest::testSimpleStaticTypes()
{
	using namespace ::Common::RustedCodec;

	const int l_inInt1 = 1;
	const int l_inInt2 = 2;
	const bool l_inBool1 = true;

	std::vector<char> l_rawWriteBuffer;
	SimpleWriteBuffer l_writeBuffer(l_rawWriteBuffer);
	RustedAbstractCoder l_inCodec(l_writeBuffer);

	l_inCodec << l_inInt1 << l_inInt2 << l_inBool1;

	const std::vector<char> l_rawReadBuffer = l_rawWriteBuffer;
	SimpleReadBuffer l_readBuffer(l_rawReadBuffer);
	RustedAbstractDecoder l_outCodec(l_readBuffer);

	int l_outInt1 = 0xf00d;
	int l_outInt2 = 0xf00d;
	bool l_outBool1 = false;

	l_outCodec >> l_outInt1 >> l_outInt2 >> l_outBool1;

	CPPUNIT_ASSERT_EQUAL(l_inInt1, l_outInt1);
	CPPUNIT_ASSERT_EQUAL(l_inInt2, l_outInt2);
	CPPUNIT_ASSERT_EQUAL(l_inBool1, l_outBool1);
}

void RustedAbstractCodecTest::testSimpleStaticAndStringTypes()
{
	using namespace ::Common::RustedCodec;

	const int l_inInt1 = 1;
	const std::string l_inStr1 = "some test string 1";
	const int l_inInt2 = 2;

	std::vector<char> l_rawWriteBuffer;
	SimpleWriteBuffer l_writeBuffer(l_rawWriteBuffer);
	RustedAbstractCoder l_inCodec(l_writeBuffer);

	l_inCodec << l_inInt1 << l_inStr1 << l_inInt2;

	const std::vector<char> l_rawReadBuffer = l_rawWriteBuffer;
	SimpleReadBuffer l_readBuffer(l_rawReadBuffer);
	RustedAbstractDecoder l_outCodec(l_readBuffer);

	int l_outInt1 = 0xf00d;
	std::string l_outStr1;
	int l_outInt2 = 0xf00d;

	l_outCodec >> l_outInt1 >> l_outStr1 >> l_outInt2;

	CPPUNIT_ASSERT_EQUAL(l_inInt1, l_outInt1);
	CPPUNIT_ASSERT(l_inStr1 == l_outStr1);
	CPPUNIT_ASSERT_EQUAL(l_inInt2, l_outInt2);
}

void RustedAbstractCodecTest::testListTypes()
{
    using namespace ::Common::RustedCodec;

    std::vector<boost::tuple<int> > inVector1;
    inVector1.push_back(boost::make_tuple(1));

    std::vector<char> rawWriteBuffer;
    SimpleWriteBuffer writeBuffer(rawWriteBuffer);
    RustedAbstractCoder inCodec(writeBuffer);

    inCodec << inVector1;

    const std::vector<char> rawReadBuffer = rawWriteBuffer;
    SimpleReadBuffer readBuffer(rawReadBuffer);
    RustedAbstractDecoder outCodec(readBuffer);

    std::vector<boost::tuple<int> > outVector1;

    outCodec >> outVector1;

    CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(1), outVector1.size());
    CPPUNIT_ASSERT(outVector1[0].get<0>() == inVector1[0].get<0>());
}

void RustedAbstractCodecTest::testListTypesWithDynamicData()
{
    using namespace ::Common::RustedCodec;

    std::vector<boost::tuple<int, std::string> > inVector1;
    inVector1.push_back(boost::make_tuple(1, "dupa"));

    std::vector<char> rawWriteBuffer;
    SimpleWriteBuffer writeBuffer(rawWriteBuffer);
    RustedAbstractCoder inCodec(writeBuffer);

    inCodec << inVector1;

    const std::vector<char> rawReadBuffer = rawWriteBuffer;
    SimpleReadBuffer readBuffer(rawReadBuffer);
    RustedAbstractDecoder outCodec(readBuffer);

    std::vector<boost::tuple<int, std::string> > outVector1;

    outCodec >> outVector1;

    CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(1), outVector1.size());
    CPPUNIT_ASSERT(outVector1[0].get<0>() == inVector1[0].get<0>());
    CPPUNIT_ASSERT(outVector1[0].get<1>() == inVector1[0].get<1>());
}


