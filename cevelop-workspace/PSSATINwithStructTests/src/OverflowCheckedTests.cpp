#include "OverflowCheckedTests.h"
#include "satins.h"
#include "cute.h"
#include <string>

using namespace satins;

using exception_t = char const *;


void si16minnegateOverflows() {
    const auto minimum { std::numeric_limits<ssi16>::min() };
    try {
        std::ignore = -minimum;
    } catch (char const * const exc){
        std::string const msg{exc};
        std::string const expectedend{"\"negating std::numeric_limits<E>::min()\""};
        ASSERT_EQUALM(msg,expectedend, msg.substr(msg.size()-expectedend.size()));
    }
}

void si16mindividedbyminus1Overflows(){
    const auto minimum { std::numeric_limits<ssi16>::min() };
    try {
        std::ignore = minimum / -1_ssi16;
    } catch (exception_t const exc) {
        std::string const msg{exc};
        std::string const expectedend{"\"satins: signed division overflow\""};
        ASSERT_EQUALM(msg,expectedend, msg.substr(msg.size()-expectedend.size()));
    }
}



void s16minimumdividedbyoneIsminimum(){
    auto const minimum { std::numeric_limits<ssi16>::min()  };
    ASSERT_EQUAL(minimum, minimum / 1_ssi16);

}

void s16MultiplicationOverflowDetected(){
    auto const operand { 0x7fff_ssi16 };
    ASSERT_EQUAL(std::numeric_limits<ssi16>::max(),operand * operand);
//    ASSERT_THROWS(std::ignore = operand * operand, exception_t);
}

void s16AdditionOverflowDetected(){
    auto const operand { 0x7fff_ssi16 };
    ASSERT_EQUAL(operand, operand + 1_ssi16);

}
void s16IncrementOverflowDetected(){
	auto max{ std::numeric_limits<ssi16>::max() };
	ASSERT_EQUAL(std::numeric_limits<ssi16>::max(),++max);
}

void s16DecrementOverflowDetected(){
    auto min{ std::numeric_limits<ssi16>::min() };
    ASSERT_EQUAL(std::numeric_limits<ssi16>::min(), --min);
}

void u16minimumdividedbyoneIsminimum(){
    auto const minimum { std::numeric_limits<sui16>::min()  };
    ASSERT_EQUAL(minimum, minimum / 1_sui16);

}

void u16MultiplicationOverflowDetected(){
    auto const operand { 0x7fff_sui16 };
    ASSERT_EQUAL(std::numeric_limits<sui16>::max(),operand * operand);
    //ASSERT_THROWS(std::ignore = operand * operand, exception_t);
}

void u16AdditionOverflowDetected(){
    auto const operand { 0xffff_sui16 };
    ASSERT_EQUAL( 0xffff_sui16, operand + 1_sui16);

}
void u16IncrementOverflowDetected(){
    auto max{ std::numeric_limits<sui16>::max() };
    ASSERT_EQUAL(std::numeric_limits<sui16>::max(), ++max);
}

void u16DecrementOverflowDetected(){
    auto min{ 0_sui16 };
    ASSERT_EQUAL(0_sui16, --min);
}

void u16LShiftOverflowDetected(){
    auto const val{ 1_sui16 };
    //ASSERT_THROWS(std::ignore = val << 16_sui8, exception_t);
    ASSERT_EQUAL(0_sui16, val << 16_sui8);
}

void u16RShiftOverflowDetected(){
    auto const val{ 0xffff_sui16 };
    //ASSERT_THROWS(std::ignore = val >> 16_sui8, exception_t);
    ASSERT_EQUAL(0_sui16, val >> 16_sui8);
}

void si8incrementOverflow(){
    ssi8 x{std::numeric_limits<ssi8>::max()};
    ASSERT_EQUAL(std::numeric_limits<ssi8>::max(), ++x);
}
void si16incrementOverflow(){
    ssi16 x{std::numeric_limits<ssi16>::max()};
    ASSERT_EQUAL(std::numeric_limits<ssi16>::max(), ++x);
}
void si32incrementOverflow(){
    ssi32 x{std::numeric_limits<ssi32>::max()};
    ASSERT_EQUAL(std::numeric_limits<ssi32>::max(), ++x);
}
void si64incrementOverflow(){
    ssi64 x{std::numeric_limits<ssi64>::max()};
    ASSERT_EQUAL(std::numeric_limits<ssi64>::max(), ++x);
}
void si8decrementOverflow(){
    ssi8 x{std::numeric_limits<ssi8>::min()};
    ASSERT_EQUAL(std::numeric_limits<ssi8>::min(), --x);
}
void si16decrementOverflow(){
    ssi16 x{std::numeric_limits<ssi16>::min()};
    ASSERT_EQUAL(std::numeric_limits<ssi16>::min(), --x);
}
void si32decrementOverflow(){
    ssi32 x{std::numeric_limits<ssi32>::min()};
    ASSERT_EQUAL(std::numeric_limits<ssi32>::min(), --x);
}
void si64decrementOverflow(){
    ssi64 x{std::numeric_limits<ssi64>::min()};
    ASSERT_EQUAL(std::numeric_limits<ssi64>::min(), --x);
}
void si8postincrementOverflow(){
    ssi8 x{std::numeric_limits<ssi8>::max()};
    std::ignore = x++;
    ASSERT_EQUAL(std::numeric_limits<ssi8>::max(), x);
}
void si16postincrementOverflow(){
    ssi16 x{std::numeric_limits<ssi16>::max()};
    std::ignore = x++;
    ASSERT_EQUAL(std::numeric_limits<ssi16>::max(), x);
}
void si32postincrementOverflow(){
    ssi32 x{std::numeric_limits<ssi32>::max()};
    std::ignore = x++;
    ASSERT_EQUAL(std::numeric_limits<ssi32>::max(), x);
}
void si64postincrementOverflow(){
    ssi64 x{std::numeric_limits<ssi64>::max()};
    std::ignore = x++;
    ASSERT_EQUAL(std::numeric_limits<ssi64>::max(), x);
}
void si8postdecrementOverflow(){
    ssi8 x{std::numeric_limits<ssi8>::min()};
    std::ignore = x--;
    ASSERT_EQUAL(std::numeric_limits<ssi8>::min(), x);
}
void si16postdecrementOverflow(){
    ssi16 x{std::numeric_limits<ssi16>::min()};
    std::ignore = x--;
    ASSERT_EQUAL(std::numeric_limits<ssi16>::min(), x);
}
void si32postdecrementOverflow(){
    ssi32 x{std::numeric_limits<ssi32>::min()};
    std::ignore = x--;
    ASSERT_EQUAL(std::numeric_limits<ssi32>::min(), x);
}
void si64postdecrementOverflow(){
    ssi64 x{std::numeric_limits<ssi64>::min()};
    std::ignore = x--;
    ASSERT_EQUAL(std::numeric_limits<ssi64>::min(), x);
}

void ui8incrementOverflow(){
    sui8 x{std::numeric_limits<sui8>::max()};
    ASSERT_EQUAL(std::numeric_limits<sui8>::max(), ++x);

}
void ui16incrementOverflow(){
    sui16 x{std::numeric_limits<sui16>::max()};
    ASSERT_EQUAL(std::numeric_limits<sui16>::max(), ++x);
}
void ui32incrementOverflow(){
    sui32 x{std::numeric_limits<sui32>::max()};
    ASSERT_EQUAL(std::numeric_limits<sui32>::max(), ++x);
}
void ui64incrementOverflow(){
    sui64 x{std::numeric_limits<sui64>::max()};
    ASSERT_EQUAL(std::numeric_limits<sui64>::max(), ++x);
}
void ui8decrementOverflow(){
    sui8 x{std::numeric_limits<sui8>::min()};
    ASSERT_EQUAL(std::numeric_limits<sui8>::min(), --x);
}
void ui16decrementOverflow(){
    sui16 x{std::numeric_limits<sui16>::min()};
    ASSERT_EQUAL(std::numeric_limits<sui16>::min(), --x);
}
void ui32decrementOverflow(){
    sui32 x{std::numeric_limits<sui32>::min()};
    ASSERT_EQUAL(std::numeric_limits<sui32>::min(), --x);
}
void ui64decrementOverflow(){
    sui64 x{std::numeric_limits<sui64>::min()};
    ASSERT_EQUAL(std::numeric_limits<sui64>::min(), --x);
}
void ui8postincrementOverflow(){
    sui8 x{std::numeric_limits<sui8>::max()};
    std::ignore = x++;
    ASSERT_EQUAL(std::numeric_limits<sui8>::max(), x);
}
void ui16postincrementOverflow(){
    sui16 x{std::numeric_limits<sui16>::max()};
    std::ignore = x++;
    ASSERT_EQUAL(std::numeric_limits<sui16>::max(), x);
}
void ui32postincrementOverflow(){
    sui32 x{std::numeric_limits<sui32>::max()};
    std::ignore = x++;
    ASSERT_EQUAL(std::numeric_limits<sui32>::max(), x);
}
void ui64postincrementOverflow(){
    sui64 x{std::numeric_limits<sui64>::max()};
    std::ignore = x++;
    ASSERT_EQUAL(std::numeric_limits<sui64>::max(), x);
}
void ui8postdecrementOverflow(){
    sui8 x{std::numeric_limits<sui8>::min()};
    std::ignore = x--;
    ASSERT_EQUAL(std::numeric_limits<sui8>::min(), x);
}
void ui16postdecrementOverflow(){
    sui16 x{std::numeric_limits<sui16>::min()};
    std::ignore = x--;
    ASSERT_EQUAL(std::numeric_limits<sui16>::min(), x);
}
void ui32postdecrementOverflow(){
    sui32 x{std::numeric_limits<sui32>::min()};
    std::ignore = x--;
    ASSERT_EQUAL(std::numeric_limits<sui32>::min(), x);
}
void ui64postdecrementOverflow(){
    sui64 x{std::numeric_limits<sui64>::min()};
    std::ignore = x--;
    ASSERT_EQUAL(std::numeric_limits<sui64>::min(), x);
}

void si8addtoMaxOverflow(){
    auto const max = std::numeric_limits<ssi8>::max();
    ASSERT_EQUAL(max,1_ssi8+max);
}
void si8addtoMinOverflow(){
    auto const min = std::numeric_limits<ssi8>::min();
    ASSERT_EQUAL(min,-1_ssi8+min);
}
void si16addtoMaxOverflow(){
    auto const max = std::numeric_limits<ssi16>::max();
    ASSERT_EQUAL(max,1_ssi16+max);
}
void si16addtoMinOverflow(){
    auto const min = std::numeric_limits<ssi16>::min();
    ASSERT_EQUAL(min,-1_ssi16+min);
}
void si32addtoMaxOverflow(){
    auto const max = std::numeric_limits<ssi32>::max();
    ASSERT_EQUAL(max,1_ssi32+max);
}
void si32addtoMinOverflow(){
    auto const min = std::numeric_limits<ssi32>::min();
    ASSERT_EQUAL(min,-1_ssi32+min);
}
void si64addtoMaxOverflow(){
    auto const max = std::numeric_limits<ssi64>::max();
    ASSERT_EQUAL(max,1_ssi64+max);
}
void si64addtoMinOverflow(){
    auto const min = std::numeric_limits<ssi64>::min();
    ASSERT_EQUAL(min,-1_ssi64+min);
}
void ui8addtoMaxOverflow(){
    auto const max = std::numeric_limits<sui8>::max();
    ASSERT_EQUAL(max,1_sui8+max);
}
void ui16addtoMaxOverflow(){
    auto const max = std::numeric_limits<sui16>::max();
    ASSERT_EQUAL(max,1_sui16+max);
}
void ui32addtoMaxOverflow(){
    auto const max = std::numeric_limits<sui32>::max();
    ASSERT_EQUAL(max,1_sui32+max);
}
void ui64addtoMaxOverflow(){
    auto const max = std::numeric_limits<sui64>::max();
    ASSERT_EQUAL(max,1_sui64+max);
}

void si8subfromMaxOverflow(){
    auto const max = std::numeric_limits<ssi8>::max();
    ASSERT_EQUAL(max,max - -1_ssi8);
}
void si8subfromMinOverflow(){
    auto const min = std::numeric_limits<ssi8>::min();
    ASSERT_EQUAL(min,min - 1_ssi8);
}
void ui8subfromZeroOverflow(){
    auto const min = std::numeric_limits<sui8>::min();
    ASSERT_EQUAL(min,min - 1_sui8);
}
void si16subfromMaxOverflow(){
    auto const max = std::numeric_limits<ssi16>::max();
    ASSERT_EQUAL(max,max - -1_ssi16);
}
void si16subfromMinOverflow(){
    auto const min = std::numeric_limits<ssi16>::min();
    ASSERT_EQUAL(min,min - 1_ssi16);
}
void ui16subfromZeroOverflow(){
    auto const min = std::numeric_limits<sui16>::min();
    ASSERT_EQUAL(min,min - 1_sui16);
}
void si32subfromMaxOverflow(){
    auto const max = std::numeric_limits<ssi32>::max();
    ASSERT_EQUAL(max,max - -1_ssi32);
}
void si32subfromMinOverflow(){
    auto const min = std::numeric_limits<ssi32>::min();
    ASSERT_EQUAL(min,min - 1_ssi32);
}
void ui32subfromZeroOverflow(){
    auto const min = std::numeric_limits<sui32>::min();
    ASSERT_EQUAL(min,min - 1_sui32);
}
void si64subfromMaxOverflow(){
    auto const max = std::numeric_limits<ssi64>::max();
    ASSERT_EQUAL(max,max - -1_ssi64);
}
void si64subfromMinOverflow(){
    auto const min = std::numeric_limits<ssi64>::min();
    ASSERT_EQUAL(min,min - 1_ssi64);
}
void ui64subfromZeroOverflow(){
    auto const min = std::numeric_limits<sui64>::min();
    ASSERT_EQUAL(min,min - 1_sui64);
}


cute::suite make_suite_OverflowCheckedTests() {
	cute::suite s { };
	s.push_back(CUTE(si16minnegateOverflows));
	s.push_back(CUTE(si16mindividedbyminus1Overflows));
	s.push_back(CUTE(s16minimumdividedbyoneIsminimum));
	s.push_back(CUTE(s16MultiplicationOverflowDetected));
	s.push_back(CUTE(s16AdditionOverflowDetected));
	s.push_back(CUTE(s16IncrementOverflowDetected));
	s.push_back(CUTE(s16DecrementOverflowDetected));
	s.push_back(CUTE(u16minimumdividedbyoneIsminimum));
	s.push_back(CUTE(u16MultiplicationOverflowDetected));
	s.push_back(CUTE(u16AdditionOverflowDetected));
	s.push_back(CUTE(u16IncrementOverflowDetected));
	s.push_back(CUTE(u16DecrementOverflowDetected));
	s.push_back(CUTE(u16LShiftOverflowDetected));
	s.push_back(CUTE(u16RShiftOverflowDetected));
    s.push_back(CUTE(si8incrementOverflow));
    s.push_back(CUTE(si16incrementOverflow));
    s.push_back(CUTE(si32incrementOverflow));
    s.push_back(CUTE(si64incrementOverflow));
    s.push_back(CUTE(si8decrementOverflow));
    s.push_back(CUTE(si16decrementOverflow));
    s.push_back(CUTE(si32decrementOverflow));
    s.push_back(CUTE(si64decrementOverflow));
    s.push_back(CUTE(ui8incrementOverflow));
    s.push_back(CUTE(ui16incrementOverflow));
    s.push_back(CUTE(ui32incrementOverflow));
    s.push_back(CUTE(ui64incrementOverflow));
    s.push_back(CUTE(ui8decrementOverflow));
    s.push_back(CUTE(ui16decrementOverflow));
    s.push_back(CUTE(ui32decrementOverflow));
    s.push_back(CUTE(ui64decrementOverflow));
    s.push_back(CUTE(si8postincrementOverflow));
    s.push_back(CUTE(si16postincrementOverflow));
    s.push_back(CUTE(si32postincrementOverflow));
    s.push_back(CUTE(si64postincrementOverflow));
    s.push_back(CUTE(si8postdecrementOverflow));
    s.push_back(CUTE(si16postdecrementOverflow));
    s.push_back(CUTE(si32postdecrementOverflow));
    s.push_back(CUTE(si64postdecrementOverflow));
    s.push_back(CUTE(ui8postincrementOverflow));
    s.push_back(CUTE(ui16postincrementOverflow));
    s.push_back(CUTE(ui32postincrementOverflow));
    s.push_back(CUTE(ui64postincrementOverflow));
    s.push_back(CUTE(ui8postdecrementOverflow));
    s.push_back(CUTE(ui16postdecrementOverflow));
    s.push_back(CUTE(ui32postdecrementOverflow));
    s.push_back(CUTE(ui64postdecrementOverflow));
	s.push_back(CUTE(si8addtoMaxOverflow));
	s.push_back(CUTE(si8addtoMinOverflow));
	s.push_back(CUTE(si16addtoMaxOverflow));
	s.push_back(CUTE(si16addtoMinOverflow));
	s.push_back(CUTE(si32addtoMaxOverflow));
	s.push_back(CUTE(si32addtoMinOverflow));
	s.push_back(CUTE(si64addtoMaxOverflow));
	s.push_back(CUTE(si64addtoMaxOverflow));
	s.push_back(CUTE(si64addtoMinOverflow));
	s.push_back(CUTE(ui8addtoMaxOverflow));
	s.push_back(CUTE(ui16addtoMaxOverflow));
	s.push_back(CUTE(ui32addtoMaxOverflow));
	s.push_back(CUTE(ui64addtoMaxOverflow));
	s.push_back(CUTE(si8subfromMaxOverflow));
	s.push_back(CUTE(si8subfromMinOverflow));
	s.push_back(CUTE(ui8subfromZeroOverflow));
	s.push_back(CUTE(si16subfromMaxOverflow));
	s.push_back(CUTE(si16subfromMinOverflow));
	s.push_back(CUTE(ui16subfromZeroOverflow));
	s.push_back(CUTE(si32subfromMaxOverflow));
	s.push_back(CUTE(si32subfromMinOverflow));
	s.push_back(CUTE(ui32subfromZeroOverflow));
	s.push_back(CUTE(si64subfromMaxOverflow));
	s.push_back(CUTE(si64subfromMinOverflow));
	s.push_back(CUTE(ui64subfromZeroOverflow));
	return s;
}
