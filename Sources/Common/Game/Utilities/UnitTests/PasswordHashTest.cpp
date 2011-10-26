#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "Game/Utilities/PasswordHash.hpp"

class PasswordHashTest: public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE (PasswordHashTest);
    CPPUNIT_TEST(testHashing);
    CPPUNIT_TEST_SUITE_END ();

public:
    void testHashing();
};

CPPUNIT_TEST_SUITE_REGISTRATION (PasswordHashTest);

void PasswordHashTest::testHashing()
{
    Common::Game::Utilities::PasswordHash hash;

    CPPUNIT_ASSERT(hash.generate("test") != hash.generate("tesT"));
    CPPUNIT_ASSERT(hash.generate("a").length() < 100);
    CPPUNIT_ASSERT(hash.generate("same") != "same");
}

