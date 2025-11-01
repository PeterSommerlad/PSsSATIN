#include "TestForZeroReturnAssertWithNDEBUG.h"
#include "cute.h"

// cannot #define NDEBUG here, because of ODR, must use consistently on all configs
#include "satins.h"
    using namespace satins;

void thisIsATestForZeroReturnAssertWithNDEBUGTest() {
    constexpr auto divisor = 0_ssi8;
    auto divident = 1_ssi8;

#ifdef NDEBUG
    ASSERT_EQUAL(0_ssi8,divident/divisor);
    ASSERT_EQUAL(0_ssi8,divident /= divisor );
#else
  #ifdef PS_ASSERT_THROWS
    ASSERT_THROWS(divident/divisor, char const *);
  #else
    #ifdef PS_TEST_TRAP
    ASSERTM("cannot test trapping without NDEBUG set, change this to true to check for assert() behavior ",false);
    divident/divisor; // Assertion failed: (r != F{} && " division by zero"), function operator/, file ../src/psssafeint.h, line 517.
    #endif
    ASSERT(divident != divisor); // dummy to prevent compile warning
  #endif
#endif
    //constexpr auto should_not_compile = 1_ssi8 / divisor; in Test.cpp
}

//static_assert(1_ssi8 / 0_ssi8 == 0_ssi8, "this should not compile");

void ModuloTestForZeroReturnAssertWithNDEBUGTest() {
    constexpr auto divisor = 0_sui8;
    auto divident = 1_sui8;

#ifdef NDEBUG
    ASSERT_EQUAL(0_sui8,divident % divisor);
    ASSERT_EQUAL(0_sui8,divident %= divisor );
#else
  #ifdef PS_ASSERT_THROWS
    ASSERT_THROWS(divident % divisor, char const *);
  #else
    #ifdef PS_TEST_TRAP
    ASSERTM("cannot test trapping without NDEBUG set, change this to true to check for assert() behavior ",false);
    divident % divisor; // Assertion failed: (r != F{} && " division by zero"), function operator/, file ../src/psssafeint.h, line 517.
    #endif
    ASSERT(divident != divisor); // dummy to prevent compile warning
  #endif
#endif

    //constexpr auto should_not_compile = 1_sui8 % divisor; in Test.cpp
}

void ShiftLeftTestForZeroReturnAssertWithNDEBUGTest() {
    constexpr auto shiftby = 8_sui8;
    auto toshift = 0xff_sui8;

#ifdef NDEBUG
    ASSERT_EQUAL(0_sui8,toshift << shiftby);
    ASSERT_EQUAL(0_sui8,toshift <<= shiftby );
#else
  #ifdef PS_ASSERT_THROWS
    ASSERT_THROWS(toshift << shiftby, char const *);
  #else
    #ifdef PS_TEST_TRAP

    ASSERTM("cannot test trapping without NDEBUG set, change this to true to check for assert() behavior ",false);
    toshift << shiftby; // Assertion failed: (r != F{} && " division by zero"), function operator/, file ../src/psssafeint.h, line 517.
    #endif
  #endif
#endif
    ASSERT(shiftby != toshift); // dummy to prevent compile warning

     //constexpr auto should_not_compile = 1_sui8 << shiftby; in Test.cpp
}
void ShiftRightTestForZeroReturnAssertWithNDEBUGTest() {
    constexpr auto shiftby = 8_sui8;
    auto toshift = 0xff_sui8;

#ifdef NDEBUG
    ASSERT_EQUAL(0_sui8,toshift >> shiftby);
    ASSERT_EQUAL(0_sui8,toshift >>= shiftby );
#else
#ifdef PS_ASSERT_THROWS
    ASSERT_THROWS(toshift >> shiftby, char const *);
#else
    #ifdef PS_TEST_TRAP

    ASSERTM("cannot test trapping without NDEBUG set, change this to true to check for assert() behavior ",false);
    toshift >> shiftby; // Assertion failed: (r != F{} && " division by zero"), function operator/, file ../src/psssafeint.h, line 517.
    #endif
  #endif
#endif
    ASSERT(shiftby != toshift); // dummy to prevent compile warning
     //constexpr auto should_not_compile = 1_sui8 >> shiftby; in Test.cpp
}


cute::suite make_suite_TestForZeroReturnAssertWithNDEBUG() {
	cute::suite s { };
	s.push_back(CUTE(thisIsATestForZeroReturnAssertWithNDEBUGTest));
	s.push_back(CUTE(ModuloTestForZeroReturnAssertWithNDEBUGTest));
	s.push_back(CUTE(ShiftLeftTestForZeroReturnAssertWithNDEBUGTest));
	s.push_back(CUTE(ShiftRightTestForZeroReturnAssertWithNDEBUGTest));
	return s;
}
