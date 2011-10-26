#ifndef COMMON_THREAD_TEST_H
#define COMMON_THREAD_TEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "Thread.hpp"

/**
 * Wait 3 secs and set public "someTestParameter" to 1
 */
class SimpleRunnableMock : public Common::IRunnable
{
public:
	SimpleRunnableMock() : someTestParameter( 0 ) { }

	void run()
	{
		Common::Thread::wait(1);
		someTestParameter = 1;
	}

	int someTestParameter;
};

class ThreadTest : public CPPUNIT_NS :: TestFixture
{
	CPPUNIT_TEST_SUITE (ThreadTest);
	CPPUNIT_TEST (simpleExecutionTest);
	//CPPUNIT_TEST (exceptionOnThreadDestructionTest);
	CPPUNIT_TEST_SUITE_END ();

protected:
        void simpleExecutionTest();
	//void exceptionOnThreadDestructionTest();
};

#endif
