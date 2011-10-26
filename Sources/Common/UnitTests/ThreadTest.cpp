#include "ThreadTest.hpp"

CPPUNIT_TEST_SUITE_REGISTRATION (ThreadTest);

void ThreadTest::simpleExecutionTest()
{
	SimpleRunnableMock runnable;
	Common::Thread thread(runnable);

	thread.start();

	// runnable is waiting 1 sec before changing the value so now
	// it should have initial (0) value

	CPPUNIT_ASSERT_EQUAL(runnable.someTestParameter, 0);
	

	// after 1 secs the value should be changed to 1, so to be sure we
	// wait 1.5 secs

	Common::Thread::wait(2);

	CPPUNIT_ASSERT_EQUAL(runnable.someTestParameter, 1);
}

/*
void ThreadTest::exceptionOnThreadDestructionTest()
{
	SimpleRunnableMock runnable;
	Common::Thread * thread = new Common::Thread(runnable);
	thread->start();

	CPPUNIT_ASSERT_EQUAL(0, runnable.someTestParameter);

	delete thread;

	Common::Thread::wait(2);

	// thread was killed so value should stay as it was

	CPPUNIT_ASSERT_EQUAL(0, runnable.someTestParameter);
}
*/

