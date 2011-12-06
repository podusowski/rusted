#include <cppunit/XmlOutputter.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>

int main (int argc, char* argv[])
{
    CPPUNIT_NS :: TestResult testresult;

    CPPUNIT_NS :: TestResultCollector collectedresults;
    testresult.addListener (&collectedresults);

    CPPUNIT_NS :: BriefTestProgressListener progress;
    testresult.addListener (&progress);

    CPPUNIT_NS :: TestRunner testrunner;
    testrunner.addTest (CPPUNIT_NS :: TestFactoryRegistry :: getRegistry ().makeTest ());
    testrunner.run (testresult);

    CPPUNIT_NS :: CompilerOutputter compileroutputter (&collectedresults, std::cerr);

    std::ofstream xmlFile("cppunit.xml");
    CppUnit::XmlOutputter xmlOutputter(&collectedresults, xmlFile);

    compileroutputter.write ();
    xmlOutputter.write();

    return collectedresults.wasSuccessful () ? 0 : 1;
}

