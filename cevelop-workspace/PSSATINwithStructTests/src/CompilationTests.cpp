#include "satins.h"

using namespace satins::literals;
using namespace satins;

namespace {
constexpr auto v1_64 {  1_ssi64 };
constexpr auto v1_32 {  1_ssi32 };
constexpr auto v1_16 {  1_ssi16 };
constexpr auto v1_8  {  1_ssi8 };
constexpr auto v2_64 {  2_ssi64 };
constexpr auto v2_32 {  2_ssi32 };
constexpr auto v2_16 {  2_ssi16 };
constexpr auto v2_8  {  2_ssi8 };
constexpr auto vminus1_64 { -1_ssi64 };
constexpr auto vminus1_32 { -1_ssi32 };
constexpr auto vminus1_16 { -1_ssi16 };
constexpr auto vminus1_8  { -1_ssi8 };

constexpr auto min_8  { std::numeric_limits<ssi8 >::min() };
constexpr auto min_16 { std::numeric_limits<ssi16>::min() };
constexpr auto min_32 { std::numeric_limits<ssi32>::min() };
constexpr auto min_64 { std::numeric_limits<ssi64>::min() };
constexpr auto max_8  { std::numeric_limits<ssi8 >::max() };
constexpr auto max_16 { std::numeric_limits<ssi16>::max() };
constexpr auto max_32 { std::numeric_limits<ssi32>::max() };
constexpr auto max_64 { std::numeric_limits<ssi64>::max() };

constexpr auto maxu_8  { std::numeric_limits<sui8 >::max() };
constexpr auto maxu_16 { std::numeric_limits<sui16>::max() };
constexpr auto maxu_32 { std::numeric_limits<sui32>::max() };
constexpr auto maxu_64 { std::numeric_limits<sui64>::max() };
constexpr auto v1u_64 {  1_sui64 };
constexpr auto v1u_32 {  1_sui32 };
constexpr auto v1u_16 {  1_sui16 };
constexpr auto v1u_8  {  1_sui8 };
constexpr auto v2u_64 {  2_sui64 };
constexpr auto v2u_32 {  2_sui32 };
constexpr auto v2u_16 {  2_sui16 };
constexpr auto v2u_8  {  2_sui8 };
}


namespace compile_checks {
using namespace satins;
template<auto ...value>
using consume_value = void;



#define concat_line_impl(A, B) A##_##B
#define concat_line(A, B) concat_line_impl(A,B)

#define check_does_compile(NOT, FROM, oper) \
namespace concat_line(NOT##_test, __LINE__) { \
        template<typename T, typename=void>\
        constexpr bool\
        expression_compiles{false};\
template<typename T> \
constexpr bool \
expression_compiles<T, consume_value<(T{} oper T{})> > {true};\
static_assert(NOT expression_compiles<FROM>, "should " #NOT " compile: " #FROM "{}" #oper #FROM "{}");\
} // namespace tag

// need to be on separate lines for disambiguation
check_does_compile(not,  ssi8 , <<  )
check_does_compile(   ,  sui8 , << )
check_does_compile(not,  ssi8 , >>  )
check_does_compile(   ,  sui8 , >> )

static_assert(0_sui8==(1_sui8<<010_sui8)); // demonstrate overflow to zero
//check_does_compile(not,  sui8 , + (1_sui8 << 010_sui8) + ) // too wide shift
check_does_compile(   ,  sui8 , + (1_sui8 << 7_sui8) + ) // not too wide shift
static_assert(0_sui8==(0x80_sui8>>010_sui8)); // demonstrate overflow to zero
//check_does_compile(not,  sui8 , + (0x80_sui8 >> 010_sui8) + ) // too wide shift
check_does_compile(   ,  sui8 , + (0x80_sui8 >> 7_sui8) + ) // not too wide shift
static_assert(0_sui8 % 0_sui8 == 0_sui8);
check_does_compile(   ,  sui8 ,  % ) // modulo 0 is defined to return 0
static_assert(1_sui8 / 0_sui8 == std::numeric_limits<sui8>::max());
check_does_compile(   ,  ssi8 ,  / ) // div 0
static_assert(std::numeric_limits<ssi8>::max() == 0_ssi8/0_ssi8);
static_assert(std::numeric_limits<ssi8>::max() == 1_ssi8/0_ssi8);
static_assert(std::numeric_limits<ssi8>::min() == (-1_ssi8)/0_ssi8);
check_does_compile(not,  ssi8 ,  % ) // modulo not working
static_assert(std::numeric_limits<sui8>::max() == 0_sui8/0_sui8);
check_does_compile(   ,  sui8 ,  / ) // div 0
static_assert(std::numeric_limits<sui8>::max() == 0_sui8/0_sui8);
static_assert(std::numeric_limits<sui8>::max() == 1_sui8/0_sui8);
check_does_compile(   ,  sui8 , +( 1_sui8  / 1_sui8)+ ) // div
check_does_compile(   ,  sui8 , +( 11_sui8  % 3_sui8)+ ) // mod
check_does_compile(not,  sui8 , + 1_ssi8 + ) // mixed
//check_does_compile(not,  sui8 , + 255_sui8 + 1_sui8 + ) // overflow detect
static_assert(std::numeric_limits<sui8>::max() == 128_sui8 * 128_sui8); // overflow
//check_does_compile(not,  sui8 , + 128_sui8 * 128_sui8 +) // overflow detect
static_assert(std::numeric_limits<ssi8>::max() == 64_ssi8 * 2_ssi8); // overflow
//check_does_compile(not,  ssi8 , + 64_ssi8 * 2_ssi8 +) // overflow detect
static_assert(std::numeric_limits<ssi8>::max() == (-127_ssi8 - 1_ssi8) * -1_ssi8); // overflow
//check_does_compile(not,  ssi8 , + (-127_ssi8 - 1_ssi8) * -1_ssi8 +) // overflow detect
check_does_compile(   ,  ssi8 , + (-127_ssi8 - 1_ssi8) * 1_ssi8 +) // no overflow
static_assert(std::numeric_limits<ssi8>::min()==(-127_ssi8 - 1_ssi8) / -1_ssi8);
//check_does_compile(not,  ssi8 , + (-127_ssi8 - 1_ssi8) / -1_ssi8 +) // overflow detect
check_does_compile(   ,  ssi8 , + (-127_ssi8 - 1_ssi8) / 1_ssi8 +) // no overflow
// demonstrate overflow detection:
//check_does_compile(not,  ssi64 , + max_64 + v1_64 +) //  overflow detect
static_assert(max_64 == max_64 + v1_64 ); //  overflow
static_assert(max_64 == max_64 + v1_32 ); //  overflow
static_assert(max_64 == max_64 + v1_16 ); //  overflow
static_assert(max_64 == max_64 + v1_8 );  //  overflow
static_assert(max_32 + v1_64 == 0x8000'0000_ssi64 );
static_assert(max_32 ==  max_32 + v1_32); //  overflow
static_assert(max_32 ==  max_32 + v1_16); //  overflow
static_assert(max_32 ==  max_32 + v1_8);  //  overflow
static_assert(max_16 + v1_64 == 0x8000_ssi64 );
static_assert(max_16 + v1_32 == 0x8000_ssi32 );
static_assert(max_16 ==  max_16 + v1_16); //  overflow
static_assert(max_16 ==  max_16 + v1_8); //  overflow
static_assert(max_8  + v1_64 == 128_ssi64 );
static_assert(max_8  + v1_32 == 128_ssi32 );
static_assert(max_8  + v1_16 == 128_ssi16 );
static_assert(max_8 ==  max_8 + v1_8); //  overflow

// unsigned addition overflow detection:
static_assert(maxu_64 ==  maxu_64 + v1u_64); //  overflow
static_assert(maxu_64 ==  maxu_64 + v1u_32); //  overflow
static_assert(maxu_64 ==  maxu_64 + v1u_16); //  overflow
static_assert(maxu_64 ==  maxu_64 + v1u_8); //  overflow

static_assert(maxu_32 + v1u_64 == 0x1'0000'0000_sui64 );
static_assert(maxu_32 ==  maxu_32 + v1u_32); //  overflow
static_assert(maxu_32 ==  maxu_32 + v1u_16); //  overflow
static_assert(maxu_32 ==  maxu_32 + v1u_8); //  overflow
static_assert(maxu_16 + v1u_64 == 0x1'0000_sui64 );
static_assert(maxu_16 + v1u_32 == 0x1'0000_sui32 );
static_assert(maxu_16 ==  maxu_16 + v1u_16); //  overflow
static_assert(maxu_16 ==  maxu_16 + v1u_8); //  overflow
static_assert(maxu_8  + v1u_64 == 256_sui64 );
static_assert(maxu_8  + v1u_32 == 256_sui32 );
static_assert(maxu_8  + v1u_16 == 256_sui16 );
static_assert(maxu_8 ==  maxu_8 + v1u_8); //  overflow

// unsigned subtraction underflow detection:
static_assert(0_sui64 - v1u_64 == 0_sui64); // underflow
static_assert(0_sui64 - v1u_32 == 0_sui64); // underflow
static_assert(0_sui64 - v1u_16 == 0_sui64); // underflow
static_assert(0_sui64 - v1u_8 == 0_sui64);  // underflow
static_assert(0_sui32 - v1u_64 == 0_sui64); // underflow
static_assert(0_sui32 - v1u_32 == 0_sui32); // underflow
static_assert(0_sui32 - v1u_16 == 0_sui32); // underflow
static_assert(0_sui32 - v1u_8 == 0_sui32);  // underflow
static_assert(0_sui16 - v1u_64 == 0_sui64); // underflow
static_assert(0_sui16 - v1u_32 == 0_sui32); // underflow
static_assert(0_sui16 - v1u_16 == 0_sui16); // underflow
static_assert(0_sui16 - v1u_8 == 0_sui16);  // underflow
static_assert(0_sui8 - v1u_64 == 0_sui64);  // underflow
static_assert(0_sui8 - v1u_32 == 0_sui32);  // underflow
static_assert(0_sui8 - v1u_16 == 0_sui16);  // underflow
static_assert(0_sui8 - v1u_8 == 0_sui8);    // underflow



// demonstrate overflow detection:
static_assert(min_64 == min_64 / vminus1_64); // clamp
static_assert(min_64 == min_64 / vminus1_32); // clamp
static_assert(min_64 == min_64 / vminus1_16); // clamp
static_assert(min_64 == min_64 / vminus1_8);  // clamp
//check_does_compile(not,  ssi64 , + min_64 / vminus1_64 +) // overflow detect
//check_does_compile(not,  ssi64 , + min_64 / vminus1_32 +) // overflow detect
//check_does_compile(not,  ssi64 , + min_64 / vminus1_16 +) // overflow detect
//check_does_compile(not,  ssi64 , + min_64 / vminus1_8 +) // overflow detect
check_does_compile(   ,  ssi64 , + min_32 / vminus1_64 +) // overflow detect
static_assert(min_32 / vminus1_64 == 0x8000'0000_ssi64 );
static_assert(min_32 == min_32 / vminus1_32);
static_assert(min_32 == min_32 / vminus1_16);
static_assert(min_32 == min_32 / vminus1_8);
//check_does_compile(not,  ssi32 , + min_32 / vminus1_32 +) // overflow detect
//check_does_compile(not,  ssi32 , + min_32 / vminus1_16 +) // overflow detect
//check_does_compile(not,  ssi32 , + min_32 / vminus1_8 +) // overflow detect
static_assert(min_16 / vminus1_64 == -static_cast<ssi64>(static_cast<ULT<decltype(min_16)>>(min_16))  );
static_assert(min_16 / vminus1_32 == 0x8000_ssi32 );
static_assert(min_16 == min_16 / vminus1_16);
static_assert(min_16 == min_16 / vminus1_8);
//check_does_compile(not,  ssi16 , + min_16 / vminus1_16 +) // overflow detect
//check_does_compile(not,  ssi16 , + min_16 / vminus1_8 +) // overflow detect
static_assert(min_8  / vminus1_64 == 128_ssi64 );
static_assert(min_8  / vminus1_32 == 128_ssi32 );
static_assert(min_8  / vminus1_16 == 128_ssi16 );
static_assert(min_8 == min_8 / vminus1_8);
//check_does_compile(not,  ssi8 , + min_8 / vminus1_8 +) // overflow detect

// demonstrate modulo overflow detection:%
// not needed, only supported for unsigned operands


// demonstrate multiplication overflow detection:
static_assert(min_64 == min_64 * v2_64);
//check_does_compile(not,  ssi64 , + min_64 * v2_64 +) // overflow detect
static_assert(min_64 == min_64 * v2_32);
//check_does_compile(not,  ssi64 , + min_64 * v2_32 +) // overflow detect
static_assert(min_64 == min_64 * v2_16);
//check_does_compile(not,  ssi64 , + min_64 * v2_16 +) // overflow detect
static_assert(min_64 == min_64 * v2_8);
//check_does_compile(not,  ssi64 , + min_64 * v2_8 +) // overflow detect
static_assert(min_32 * v2_64 == -0x1'0000'0000_ssi64 );
static_assert(min_32 == min_32 * v2_32);
static_assert(min_32 == min_32 * v2_16);
static_assert(min_32 == min_32 * v2_8);
//check_does_compile(not,  ssi32 , + min_32 * v2_32 +) // overflow detect
//check_does_compile(not,  ssi32 , + min_32 * v2_16 +) // overflow detect
//check_does_compile(not,  ssi32 , + min_32 * v2_8 +) // overflow detect
static_assert(min_16 * v2_64 == -0x1'0000_ssi64  );
static_assert(min_16 * v2_32 == -0x1'0000_ssi32 );
static_assert(min_16 == min_16 * v2_16);
static_assert(min_16 == min_16 * v2_8);
//check_does_compile(not,  ssi16 , + min_16 * v2_16 +) // overflow detect
//check_does_compile(not,  ssi16 , + min_16 * v2_8 +) // overflow detect
static_assert(min_8  * v2_64 == -256_ssi64 );
static_assert(min_8  * v2_32 == -256_ssi32 );
static_assert(min_8  * v2_16 == -256_ssi16 );
static_assert(min_8 == min_8 * v2_8);
//check_does_compile(not,  ssi8 , + min_8 * v2_8 +) // overflow detect

// demonstrate unsigned multiplication overflow detection:
static_assert(maxu_64 == maxu_64 * v2u_64);
static_assert(maxu_64 == maxu_64 * v2u_32);
static_assert(maxu_64 == maxu_64 * v2u_16);
static_assert(maxu_64 == maxu_64 * v2u_8);
//check_does_compile(not,  sui64 , + maxu_64 * v2u_64 +) // overflow detect
//check_does_compile(not,  sui64 , + maxu_64 * v2u_32 +) // overflow detect
//check_does_compile(not,  sui64 , + maxu_64 * v2u_16 +) // overflow detect
//check_does_compile(not,  sui64 , + maxu_64 * v2u_8 +) // overflow detect
static_assert(maxu_32 * v2u_64 + 2_sui64 == 0x2'0000'0000_sui64 );
static_assert(maxu_32 == maxu_32 * v2u_32);
static_assert(maxu_32 == maxu_32 * v2u_16);
static_assert(maxu_32 == maxu_32 * v2u_8);
//check_does_compile(not,  sui32 , + maxu_32 * v2u_32 +) // overflow detect
//check_does_compile(not,  sui32 , + maxu_32 * v2u_16 +) // overflow detect
//check_does_compile(not,  sui32 , + maxu_32 * v2u_8 +) // overflow detect
static_assert(maxu_16 * v2u_64 == 0x1'fffe_sui64  );
static_assert(maxu_16 * v2u_32 == 0x1'fffe_sui32 );
static_assert(maxu_16 == maxu_16 * v2u_16);
static_assert(maxu_16 == maxu_16 * v2u_8);
//check_does_compile(not,  sui16 , + maxu_16 * v2u_16 +) // overflow detect
//check_does_compile(not,  sui16 , + maxu_16 * v2u_8 +) // overflow detect
static_assert(maxu_8  * v2u_64 == 510_sui64 );
static_assert(maxu_8  * v2u_32 == 510_sui32 );
static_assert(maxu_8  * v2u_16 == 510_sui16 );
static_assert(maxu_8 == maxu_8 * v2u_8);
//check_does_compile(not,  sui8 , + maxu_8 * v2u_8 +) // overflow detect

static_assert(maxu_16 == 0xffff_sui16 * 0xffff_sui16);
//check_does_compile(not,  sui16 , + 0xffff_sui16 * 0xffff_sui16 +) // overflow detect
//static_assert(promote_keep_signedness(0xffff_sui16 * 0xffff_sui16) == 0x1u); // wraps
static_assert(maxu_8 == 0xff_sui8 * 0xff_sui8);
//check_does_compile(not,  sui8 , + 0xff_sui8 * 0xff_sui8 +) // overflow detect
//static_assert(0xff_sui8 * 0xff_sui8 == 1_sui8);


// LShift operations (on unsigned)

static_assert(2_sui8  == (v1u_8  << v1u_8));
static_assert(2_sui16 == (v1u_16 << v1u_8));
static_assert(2_sui32 == (v1u_32 << v1u_8));
static_assert(2_sui64 == (v1u_64 << v1u_8));

// Overflow causes 0, not UB or errors:
//check_does_compile(not,  sui8 , + v1u_8 << 8_sui8 +) // overflow detect
static_assert(0_sui8 == v1u_8 << 8_sui8);
//check_does_compile(not,  sui16 , + v1u_16 << 16_sui8 +) // overflow detect
static_assert(0_sui16 == v1u_16 << 16_sui8);
//check_does_compile(not,  sui32 , + v1u_32 << 32_sui8 +) // overflow detect
static_assert(0_sui32 == v1u_32 << 32_sui8);
//check_does_compile(not,  sui64 , + v1u_64 << 64_sui8 +) // overflow detect
static_assert(0_sui64 == v1u_64 << 64_sui8);


// RShift operations (on unsigned)

static_assert(0_sui8  == (v1u_8  >> v1u_8));
static_assert(0_sui16 == (v1u_16 >> v1u_8));
static_assert(0_sui32 == (v1u_32 >> v1u_8));
static_assert(0_sui64 == (v1u_64 >> v1u_8));

// Overflow causes 0, not UB or errors:
//check_does_compile(not,  sui8 , + maxu_8 >> 8_sui8 +) // overflow detect
static_assert(0_sui8 == maxu_8 >> 8_sui8);
//check_does_compile(not,  sui16 , + maxu_16 >> 16_sui8 +) // overflow detect
static_assert(0_sui16 == maxu_16 >> 16_sui8);
//check_does_compile(not,  sui32 , + maxu_32 >> 32_sui8 +) // overflow detect
static_assert(0_sui32 == maxu_32 >> 32_sui8);
//check_does_compile(not,  sui64 , + maxu_64 >> 64_sui8 +) // overflow detect
static_assert(0_sui64 == maxu_64 >> 64_sui8);
//check_does_compile(not,  sui64 , + maxu_64 >> maxu_64 +) // overflow detect

// the following does not compile due to signed integer overflow on 32bit int
//static_assert(static_cast<uint16_t>(0xffffu)* static_cast<uint16_t>(0xffffu));

//check_does_compile(not,  ssi32 , + 0x7fff'ffff_ssi32+2_ssi32 +) // overflow detect
static_assert(0x7fff'ffff_ssi32+2_ssi32 == 0x7fff'ffff_ssi32);
//static_assert(0x7fff'ffff + 2); // doesn't compile, integer overflow
static_assert(std::numeric_limits<ssi32>::min() == (-0x7fff'ffff_ssi32 - 2_ssi32));
//check_does_compile(not,  ssi32 , + (-0x7fff'ffff_ssi32 - 2_ssi32) +) // overflow detect
//static_assert(-0x7fff'ffff_ssi32 - 2_ssi32 == 0x7fff'ffff_ssi32);
//static_assert(-0x7fff'ffff - 2); // doesn't compile, integer overflow


check_does_compile(   ,  ssi32, + std::numeric_limits<ssi32>::min() / 1_ssi32  +) // no overflow
//check_does_compile(not,  ssi32, + std::numeric_limits<ssi32>::min() / -1_ssi32  +) // overflow detected
static_assert(std::numeric_limits<ssi32>::min() / 1_ssi32 == std::numeric_limits<ssi32>::min()); // wraps
static_assert(std::numeric_limits<ssi32>::min() / -1_ssi32 == std::numeric_limits<ssi32>::min()); // wraps

// doesn't seem to work with gcc, but with Apple clang, not sure which is more correct.
int from_int(...); // cause non-matching code below to SFINAE


check_does_compile(not ,  ssi8, + from_int(' ')  +) // invalid conversion
check_does_compile(not ,  sui8, + from_int(u' ')  +) // invalid conversion
check_does_compile(not ,  sui32, + from_int(U' ')  +) // invalid conversion
check_does_compile(not ,  sui16, + from_int(L' ')  +) // invalid conversion
check_does_compile(not ,  sui8, + from_int(true)  +) // invalid conversion
//static_assert(32_sui8 == from_int(' ')); // does not compile
//static_assert(32_sui8 == from_int(u' ')); // does not compile
//static_assert(32_sui8 == from_int(U' ')); // does not compile
//static_assert(32_sui8 == from_int(L' ')); // does not compile
//static_assert(1_sui8 == from_int_to<ui8>(true)); // does not compile

// to signed from signed

check_does_compile( ,  ssi8, + from_int_to<ssi8>(42)  +) // ok conversion
check_does_compile( ,  ssi8, + from_int_to<ssi8>(std::numeric_limits<int8_t>::min()+0)  +) // ok conversion
check_does_compile( ,  ssi8, + from_int_to<ssi8>(std::numeric_limits<int8_t>::max()+0)  +) // ok conversion
static_assert(std::numeric_limits<ssi8>::min() == from_int_to<ssi8>(std::numeric_limits<int8_t>::min()-1));
static_assert(std::numeric_limits<ssi8>::max() == from_int_to<ssi8>(std::numeric_limits<int8_t>::max()+1));
check_does_compile(    ,  ssi8, + from_int_to<ssi8>(std::numeric_limits<int8_t>::min()-1)  +) // overflow conversion
check_does_compile(    ,  ssi8, + from_int_to<ssi8>(std::numeric_limits<int8_t>::max()+1)  +) // overflow conversion

check_does_compile(    ,  ssi16, + from_int_to<ssi16>(42)  +) // ok conversion
check_does_compile(    ,  ssi16, + from_int_to<ssi16>(std::numeric_limits<int16_t>::min()+0)  +) // ok conversion
check_does_compile(    ,  ssi16, + from_int_to<ssi16>(std::numeric_limits<int16_t>::max()+0)  +) // ok conversion
static_assert(std::numeric_limits<ssi16>::min() == from_int_to<ssi16>(std::numeric_limits<int16_t>::min()-1));
static_assert(std::numeric_limits<ssi16>::max() == from_int_to<ssi16>(std::numeric_limits<int16_t>::max()+1));
check_does_compile(    ,  ssi16, + from_int_to<ssi16>(std::numeric_limits<int16_t>::min()-1)  +) // overflow conversion
check_does_compile(    ,  ssi16, + from_int_to<ssi16>(std::numeric_limits<int16_t>::max()+1)  +) // overflow conversion

check_does_compile(    ,  ssi32, + from_int_to<ssi32>(42)  +) // ok conversion
check_does_compile(    ,  ssi32, + from_int_to<ssi32>((int64_t)std::numeric_limits<int32_t>::min()+0)  +) // ok conversion
check_does_compile(    ,  ssi32, + from_int_to<ssi32>((int64_t)std::numeric_limits<int32_t>::max()+0)  +) // ok conversion
static_assert(std::numeric_limits<ssi32>::min() == from_int_to<ssi32>((int64_t)std::numeric_limits<int32_t>::min()-1));
static_assert(std::numeric_limits<ssi32>::max() == from_int_to<ssi32>((int64_t)std::numeric_limits<int32_t>::max()+1));
check_does_compile(    ,  ssi32, + from_int_to<ssi32>((int64_t)std::numeric_limits<int32_t>::min()-1)  +) // overflow conversion
check_does_compile(    ,  ssi32, + from_int_to<ssi32>((int64_t)std::numeric_limits<int32_t>::max()+1)  +) // overflow conversion

check_does_compile(    ,  ssi64, + from_int_to<ssi64>(42)  +) // ok conversion
check_does_compile(    ,  ssi64, + from_int_to<ssi64>(std::numeric_limits<int64_t>::min()+0)  +) // ok conversion
check_does_compile(    ,  ssi64, + from_int_to<ssi64>(std::numeric_limits<int64_t>::max()+0)  +) // ok conversion
check_does_compile(not ,  ssi64, + from_int_to<ssi64>(std::numeric_limits<int64_t>::min()-1)  +) // check would cause UB
check_does_compile(not ,  ssi64, + from_int_to<ssi64>(std::numeric_limits<int64_t>::max()+1)  +) // check fails due to UB


// to signed from unsigned
check_does_compile(    ,  ssi8, + from_int_to<ssi8>(42ull)  +) // ok conversion
check_does_compile(    ,  ssi8, + from_int_to<ssi8>(std::numeric_limits<int8_t>::min()+0)  +) // from unsigned too large
static_assert(std::numeric_limits<ssi8>::max() == from_int_to<ssi8>(std::numeric_limits<int8_t>::min()+0u)); // silent signed to unsigned conversion
check_does_compile(    ,  ssi8, + from_int_to<ssi8>(std::numeric_limits<int8_t>::min()+0u)  +) // from unsigned too large
static_assert(std::numeric_limits<ssi8>::max() == from_int_to<ssi8>(std::numeric_limits<int8_t>::min()-1u)); // silent signed to unsigned conversion
check_does_compile(    ,  ssi8, + from_int_to<ssi8>(std::numeric_limits<int8_t>::min()-1u)  +) //
check_does_compile(    ,  ssi8, + from_int_to<ssi8>(std::numeric_limits<int8_t>::max()+0u)  +) // ok conversion
static_assert(std::numeric_limits<ssi8>::max() == from_int_to<ssi8>(std::numeric_limits<int8_t>::max()+1u)); // silent signed to unsigned conversion
check_does_compile(    ,  ssi8, + from_int_to<ssi8>(std::numeric_limits<int8_t>::max()+1u)  +) // overflow from unsigned conversion
static_assert(std::numeric_limits<ssi8>::max() == from_int_to<ssi8>(std::numeric_limits<uint8_t>::max())); // silent signed to unsigned conversion
check_does_compile(    ,  ssi8, + from_int_to<ssi8>(std::numeric_limits<uint8_t>::max())  +) // too big
static_assert(std::numeric_limits<ssi8>::max() == from_int_to<ssi8>(std::numeric_limits<uint8_t>::min()-1u)); // silent signed to unsigned conversion
check_does_compile(    ,  ssi8, + from_int_to<ssi8>(std::numeric_limits<uint8_t>::min()-1u)  +) // promotion to int no overflow
check_does_compile(    ,  ssi8, + from_int_to<ssi8>(std::numeric_limits<uint8_t>::max() + std::numeric_limits<int8_t>::min())  +) // OK

check_does_compile(    ,  ssi16, + from_int_to<ssi16>(42ull)  +) // ok conversion
check_does_compile(    ,  ssi16, + from_int_to<ssi16>(std::numeric_limits<int16_t>::min()+0ll)  +) // from unsigned too large
static_assert(std::numeric_limits<ssi16>::max() == from_int_to<ssi16>(std::numeric_limits<int16_t>::min()+0u)); // silent signed to unsigned conversion
check_does_compile(    ,  ssi16, + from_int_to<ssi16>(std::numeric_limits<int16_t>::min()+0u)  +) // from unsigned too large
static_assert(std::numeric_limits<ssi16>::max() == from_int_to<ssi16>(std::numeric_limits<int16_t>::min()-1u)); // silent signed to unsigned conversion
check_does_compile(    ,  ssi16, + from_int_to<ssi16>(std::numeric_limits<int16_t>::min()-1u)  +) // ok from unsigned too large
check_does_compile(    ,  ssi16, + from_int_to<ssi16>(std::numeric_limits<int16_t>::max()+0ull)  +) // ok conversion
static_assert(std::numeric_limits<ssi16>::max() == from_int_to<ssi16>(std::numeric_limits<int16_t>::max()+1ull)); // silent signed to unsigned conversion
check_does_compile(    ,  ssi16, + from_int_to<ssi16>(std::numeric_limits<int16_t>::max()+1ull)  +) // overflow from unsigned conversion
static_assert(std::numeric_limits<ssi16>::max() == from_int_to<ssi16>(std::numeric_limits<uint16_t>::max())); // silent signed to unsigned conversion
check_does_compile(    ,  ssi16, + from_int_to<ssi16>(std::numeric_limits<uint16_t>::max())  +) // too big
static_assert(std::numeric_limits<ssi16>::max() == from_int_to<ssi16>(std::numeric_limits<uint16_t>::min()-1u)); // silent signed to unsigned conversion
check_does_compile(    ,  ssi16, + from_int_to<ssi16>(std::numeric_limits<uint16_t>::min()-1u)  +) // unsigned overflow leads to too big value
check_does_compile(    ,  ssi16, + from_int_to<ssi16>(std::numeric_limits<uint16_t>::max() + std::numeric_limits<int16_t>::min())  +) // OKdue to integral promotion

check_does_compile(    ,  ssi32, + from_int_to<ssi32>(42ull)  +) // ok conversion
check_does_compile(    ,  ssi32, + from_int_to<ssi32>(std::numeric_limits<int32_t>::min()+0)  +) // from unsigned too large
static_assert(std::numeric_limits<ssi32>::max() == from_int_to<ssi32>(std::numeric_limits<int32_t>::min()+0u)); // silent signed to unsigned conversion
check_does_compile(    ,  ssi32, + from_int_to<ssi32>(std::numeric_limits<int32_t>::min()+0u)  +) // from unsigned too large
check_does_compile(    ,  ssi32, + from_int_to<ssi32>(std::numeric_limits<int32_t>::min()-1u)  +) // ok from unsigned too large
check_does_compile(    ,  ssi32, + from_int_to<ssi32>(std::numeric_limits<int32_t>::max()+0u)  +) // ok conversion
static_assert(std::numeric_limits<ssi32>::max() == from_int_to<ssi32>(std::numeric_limits<int32_t>::max()+1u)); // silent signed to unsigned conversion
check_does_compile(    ,  ssi32, + from_int_to<ssi32>(std::numeric_limits<int32_t>::max()+1u)  +) // overflow from unsigned conversion
static_assert(std::numeric_limits<ssi32>::max() == from_int_to<ssi32>(std::numeric_limits<uint32_t>::max())); // silent signed to unsigned conversion
check_does_compile(    ,  ssi32, + from_int_to<ssi32>(std::numeric_limits<uint32_t>::max())  +) // too big
static_assert(std::numeric_limits<ssi32>::max() == from_int_to<ssi32>(std::numeric_limits<uint32_t>::min()-1)); // silent signed to unsigned conversion
check_does_compile(    ,  ssi32, + from_int_to<ssi32>(std::numeric_limits<uint32_t>::min()-1)  +) // unsigned overflow leads to too big value
check_does_compile(    ,  ssi32, + from_int_to<ssi32>(std::numeric_limits<uint32_t>::max()-std::numeric_limits<int32_t>::min())  +) // OK

check_does_compile(    ,  ssi64, + from_int_to<ssi64>(42ull)  +) // ok conversion
check_does_compile(    ,  ssi64, + from_int_to<ssi64>(std::numeric_limits<int64_t>::min()+0ll)  +) // from unsigned too large
static_assert(std::numeric_limits<ssi64>::max() == from_int_to<ssi64>(std::numeric_limits<int64_t>::min()+0ull)); // silent signed to unsigned conversion
check_does_compile(    ,  ssi64, + from_int_to<ssi64>(std::numeric_limits<int64_t>::min()+0ull)  +) // from unsigned too large
check_does_compile(    ,  ssi64, + from_int_to<ssi64>(std::numeric_limits<int64_t>::min()-1ull)  +) // ok from unsigned too large
check_does_compile(    ,  ssi64, + from_int_to<ssi64>(std::numeric_limits<int64_t>::max()+0ull)  +) // ok conversion
static_assert(std::numeric_limits<ssi64>::max() == from_int_to<ssi64>(std::numeric_limits<int64_t>::max()+1ull)); // silent signed to unsigned conversion
check_does_compile(    ,  ssi64, + from_int_to<ssi64>(std::numeric_limits<int64_t>::max()+1ull)  +) // overflow from unsigned conversion
static_assert(std::numeric_limits<ssi64>::max() == from_int_to<ssi64>(std::numeric_limits<uint64_t>::max())); // silent signed to unsigned conversion
check_does_compile(    ,  ssi64, + from_int_to<ssi64>(std::numeric_limits<uint64_t>::max())  +) // too big
static_assert(std::numeric_limits<ssi64>::max() == from_int_to<ssi64>(std::numeric_limits<uint64_t>::min()-1)); // silent signed to unsigned conversion
check_does_compile(    ,  ssi64, + from_int_to<ssi64>(std::numeric_limits<uint64_t>::min()-1)  +) // unsigned overflow leads to too big value
check_does_compile(    ,  ssi64, + from_int_to<ssi64>(std::numeric_limits<uint64_t>::max()-std::numeric_limits<int64_t>::min())  +) // OK

// to unsigned from (mostly) unsigned

static_assert(std::numeric_limits<sui8>::min() == from_int_to<sui8>(-1));
check_does_compile(    ,  sui8, + from_int_to<sui8>(-1)  +) // clamp to range
check_does_compile(    ,  sui8, + from_int_to<sui8>(42)  +) // ok conversion
check_does_compile(    ,  sui8, + from_int_to<sui8>(std::numeric_limits<uint8_t>::min()+0)  +) // ok conversion
check_does_compile(    ,  sui8, + from_int_to<sui8>(std::numeric_limits<uint8_t>::max()+0)  +) // ok conversion
static_assert(std::numeric_limits<sui8>::min() == from_int_to<sui8>(std::numeric_limits<uint8_t>::min()-1));
check_does_compile(    ,  sui8, + from_int_to<sui8>(std::numeric_limits<uint8_t>::min()-1)  +) // overflow clamp
static_assert(std::numeric_limits<sui8>::max() == from_int_to<sui8>(std::numeric_limits<uint8_t>::max()+1));
check_does_compile(    ,  sui8, + from_int_to<sui8>(std::numeric_limits<uint8_t>::max()+1)  +) // overflow clamp

static_assert(std::numeric_limits<sui16>::min() == from_int_to<sui16>(-1));
check_does_compile(    ,  sui16, + from_int_to<sui16>(-1)  +) // clamp to range
check_does_compile(    ,  sui16, + from_int_to<sui16>(42)  +) // ok conversion
check_does_compile(    ,  sui16, + from_int_to<sui16>(std::numeric_limits<uint16_t>::min()+0)  +) // ok conversion
check_does_compile(    ,  sui16, + from_int_to<sui16>(std::numeric_limits<uint16_t>::max()+0)  +) // ok conversion
static_assert(std::numeric_limits<sui16>::min() == from_int_to<sui16>(std::numeric_limits<uint16_t>::min()-1));
check_does_compile(    ,  sui16, + from_int_to<sui16>(std::numeric_limits<uint16_t>::min()-1)  +) // overflow conversion
static_assert(std::numeric_limits<sui16>::max() == from_int_to<sui16>(std::numeric_limits<uint16_t>::max()+1));
check_does_compile(    ,  sui16, + from_int_to<sui16>(std::numeric_limits<uint16_t>::max()+1)  +) // overflow conversion

static_assert(std::numeric_limits<sui32>::min() == from_int_to<sui32>(-1));
check_does_compile(    ,  sui32, + from_int_to<sui32>(-1)  +) // clamp to range
check_does_compile(    ,  sui32, + from_int_to<sui32>(42)  +) // ok conversion
check_does_compile(    ,  sui32, + from_int_to<sui32>(std::numeric_limits<uint32_t>::min()+0)  +) // ok conversion
check_does_compile(    ,  sui32, + from_int_to<sui32>(std::numeric_limits<uint32_t>::max()+0)  +) // ok conversion
check_does_compile(    ,  sui32, + from_int_to<sui32>(std::numeric_limits<uint32_t>::min()-1)  +) // expression wraps, OK conversion
check_does_compile(    ,  sui32, + from_int_to<sui32>(std::numeric_limits<uint32_t>::max()+1)  +) // expression wraps, OK conversion

static_assert(std::numeric_limits<sui64>::min() == from_int_to<sui64>(-1));
check_does_compile(    ,  sui64, + from_int_to<sui64>(-1)  +) // clamp to range
check_does_compile(    ,  sui64, + from_int_to<sui64>(42)  +) // ok conversion
check_does_compile(    ,  sui64, + from_int_to<sui64>(std::numeric_limits<uint64_t>::min()+0)  +) // ok conversion
check_does_compile(    ,  sui64, + from_int_to<sui64>(std::numeric_limits<uint64_t>::max()+0)  +) // ok conversion
check_does_compile(    ,  sui64, + from_int_to<sui64>(std::numeric_limits<uint64_t>::min()-1)  +) // expression wraps, OK conversion
check_does_compile(    ,  sui64, + from_int_to<sui64>(std::numeric_limits<uint64_t>::max()+1)  +) // expression wraps, OK conversion

// to unsigned from signed

check_does_compile(    ,  sui8, + from_int_to<sui8>(42ll)  +) // ok conversion
static_assert(std::numeric_limits<sui8>::min() == from_int_to<sui8>(std::numeric_limits<int8_t>::min()+0)); // clamp
check_does_compile(    ,  sui8, + from_int_to<sui8>(std::numeric_limits<int8_t>::min()+0)  +) // from unsigned too large
static_assert(std::numeric_limits<sui8>::max() == from_int_to<sui8>(std::numeric_limits<int8_t>::min()+0u)); // clamp
check_does_compile(    ,  sui8, + from_int_to<sui8>(std::numeric_limits<int8_t>::min()+0u)  +) // from unsigned too large
static_assert(std::numeric_limits<sui8>::max() == from_int_to<sui8>(std::numeric_limits<int8_t>::min()-1u)); // clamp
check_does_compile(    ,  sui8, + from_int_to<sui8>(std::numeric_limits<int8_t>::min()-1u)  +) //
check_does_compile(    ,  sui8, + from_int_to<sui8>(std::numeric_limits<int8_t>::max()+0u)  +) // ok conversion
check_does_compile(    ,  sui8, + from_int_to<sui8>(std::numeric_limits<int8_t>::max()+1u)  +) // overflow from unsigned conversion
check_does_compile(    ,  sui8, + from_int_to<sui8>(std::numeric_limits<uint8_t>::max())  +) // ok
static_assert(std::numeric_limits<sui8>::max() == from_int_to<sui8>(std::numeric_limits<uint8_t>::max()+1u)); // clamp
check_does_compile(    ,  sui8, + from_int_to<sui8>(std::numeric_limits<uint8_t>::max()+1u)  +) // not ok
static_assert(std::numeric_limits<sui8>::max() == from_int_to<sui8>(std::numeric_limits<uint8_t>::min()-1u)); // clamp
check_does_compile(    ,  sui8, + from_int_to<sui8>(std::numeric_limits<uint8_t>::min()-1u)  +) // unsigned overflow leads to too big value
check_does_compile(    ,  sui8, + from_int_to<sui8>(std::numeric_limits<uint8_t>::max() + std::numeric_limits<int8_t>::min())  +) // OKdue to integral promotion

check_does_compile(    ,  sui16, + from_int_to<sui16>(42ll)  +) // ok conversion
static_assert(std::numeric_limits<sui16>::min() == from_int_to<sui16>(std::numeric_limits<int16_t>::min()+0ll)); // clamp
check_does_compile(    ,  sui16, + from_int_to<sui16>(std::numeric_limits<int16_t>::min()+0ll)  +) // from unsigned too large
static_assert(std::numeric_limits<sui16>::max() == from_int_to<sui16>(std::numeric_limits<int16_t>::min()+0u)); // clamp
check_does_compile(    ,  sui16, + from_int_to<sui16>(std::numeric_limits<int16_t>::min()+0u)  +) // from unsigned too large
static_assert(std::numeric_limits<sui16>::max() == from_int_to<sui16>(std::numeric_limits<int16_t>::min()-1u)); // clamp
check_does_compile(    ,  sui16, + from_int_to<sui16>(std::numeric_limits<int16_t>::min()-1u)  +) // ok from unsigned too large
check_does_compile(    ,  sui16, + from_int_to<sui16>(std::numeric_limits<int16_t>::max()+0ull)  +) // ok conversion
check_does_compile(    ,  sui16, + from_int_to<sui16>(std::numeric_limits<int16_t>::max()+1ull)  +) // overflow from unsigned conversion
check_does_compile(    ,  sui16, + from_int_to<sui16>(std::numeric_limits<uint16_t>::max())  +) // too big
static_assert(std::numeric_limits<sui16>::max() == from_int_to<sui16>(std::numeric_limits<uint16_t>::max() +1u)); // clamp
check_does_compile(    ,  sui16, + from_int_to<sui16>(std::numeric_limits<uint16_t>::max() +1u)  +) // too big
static_assert(std::numeric_limits<sui16>::max() == from_int_to<sui16>(std::numeric_limits<uint16_t>::min()-1u)); // clamp
check_does_compile(    ,  sui16, + from_int_to<sui16>(std::numeric_limits<uint16_t>::min()-1u)  +) // unsigned overflow leads to too big value
check_does_compile(    ,  sui16, + from_int_to<sui16>(std::numeric_limits<uint16_t>::max() + std::numeric_limits<int16_t>::min())  +) // OKdue to integral promotion

check_does_compile(    ,  sui32, + from_int_to<sui32>(42ll)  +) // ok conversion
static_assert(std::numeric_limits<sui32>::min() == from_int_to<sui32>(std::numeric_limits<int32_t>::min())); // clamp
check_does_compile(    ,  sui32, + from_int_to<sui32>(std::numeric_limits<int32_t>::min())  +) // from signed too large
check_does_compile(    ,  sui32, + from_int_to<sui32>(std::numeric_limits<int32_t>::min()+0u)  +) // from unsigned too large
check_does_compile(    ,  sui32, + from_int_to<sui32>(std::numeric_limits<int32_t>::min()-1u)  +) // ok from unsigned too large
check_does_compile(    ,  sui32, + from_int_to<sui32>(std::numeric_limits<int32_t>::max()+0u)  +) // ok conversion
check_does_compile(    ,  sui32, + from_int_to<sui32>(std::numeric_limits<int32_t>::max()+1u)  +) // overflow from unsigned conversion
check_does_compile(    ,  sui32, + from_int_to<sui32>(std::numeric_limits<uint32_t>::max())  +) // too big
static_assert(std::numeric_limits<sui32>::min() == from_int_to<sui32>(std::numeric_limits<uint32_t>::min()-1ll)); // clamp
check_does_compile(    ,  sui32, + from_int_to<sui32>(std::numeric_limits<uint32_t>::min()-1ll)  +) // unsigned overflow leads to too big value
check_does_compile(    ,  sui32, + from_int_to<sui32>(std::numeric_limits<uint32_t>::max()-std::numeric_limits<int32_t>::min())  +) // OK

check_does_compile(    ,  sui64, + from_int_to<sui64>(42ll)  +) // ok conversion
static_assert(std::numeric_limits<sui64>::min() == from_int_to<sui64>(std::numeric_limits<int64_t>::min())); // clamp
check_does_compile(    ,  sui64, + from_int_to<sui64>(std::numeric_limits<int64_t>::min())  +) // from unsigned too large
static_assert(std::numeric_limits<sui64>::min() == from_int_to<sui64>(-1)); // clamp
check_does_compile(    ,  sui64, + from_int_to<sui64>(-1)  +) // negative to unsigned fails
check_does_compile(    ,  sui64, + from_int_to<sui64>(std::numeric_limits<int64_t>::min()+0ull)  +) // from unsigned too large
check_does_compile(    ,  sui64, + from_int_to<sui64>(std::numeric_limits<int64_t>::min()-1ull)  +) // ok from unsigned too large
check_does_compile(    ,  sui64, + from_int_to<sui64>(std::numeric_limits<int64_t>::max())  +) // ok conversion
check_does_compile(    ,  sui64, + from_int_to<sui64>(std::numeric_limits<int64_t>::max()+1ull)  +) // unsigned overflow always OK
check_does_compile(    ,  sui64, + from_int_to<sui64>(std::numeric_limits<uint64_t>::max())  +) // OK
check_does_compile(    ,  sui64, + from_int_to<sui64>(std::numeric_limits<uint64_t>::min()-1ll)  +) // unsigned overflow always OK
check_does_compile(    ,  sui64, + from_int_to<sui64>(std::numeric_limits<uint64_t>::max()-std::numeric_limits<int64_t>::min())  +) // OK

// check negation detection
check_does_compile(    ,  ssi8, +  (- max_8)  +) // ok conversion
static_assert(max_8 == -min_8); // overflow
//check_does_compile(not ,  ssi8, +  (- min_8)  +) // ok conversion
check_does_compile(    ,  ssi16, +  (- max_16)  +) // ok conversion
static_assert(max_16 == -min_16); // overflow
//check_does_compile(not ,  ssi16, +  (- min_16)  +) // ok conversion
check_does_compile(    ,  ssi32, +  (- max_32)  +) // ok conversion
static_assert(max_32 == -min_32); // overflow
//check_does_compile(not ,  ssi32, +  (- min_32)  +) // ok conversion
check_does_compile(    ,  ssi64, +  (- max_64)  +) // ok conversion
static_assert(max_64 == -min_64); // overflow
//check_does_compile(not ,  ssi64, +  (- min_64)  +) // ok conversion
// negation of unsigned types is not available due to concept

// check increment/decrement continue here.... (see below, cannot be at compile time)
// check add
static_assert(max_8 + 1_ssi8 == std::numeric_limits<ssi8>::max());// overflow
static_assert(max_16 + 1_ssi8 == std::numeric_limits<ssi16>::max());// overflow
static_assert(max_32 + 1_ssi8 == std::numeric_limits<ssi32>::max());// overflow
static_assert(max_64 + 1_ssi8 == std::numeric_limits<ssi64>::max());// overflow
static_assert(min_8 + -1_ssi8 == std::numeric_limits<ssi8>::min());// overflow
static_assert(min_16 + -1_ssi8 == std::numeric_limits<ssi16>::min());// overflow
static_assert(min_32 + -1_ssi8 == std::numeric_limits<ssi32>::min());// overflow
static_assert(min_64 + -1_ssi8 == std::numeric_limits<ssi64>::min());// overflow
check_does_compile(not ,  ssi8,  +   1_sui8  +) // mixing signedness
check_does_compile(not ,  ssi16, +   1_sui8  +) // mixing signedness
check_does_compile(not ,  ssi32, +   1_sui8  +) // mixing signedness
check_does_compile(not ,  ssi64, +   1_sui8  +) // mixing signedness
static_assert(maxu_8 + 1_sui8 == std::numeric_limits<sui8>::max());// overflow
static_assert(maxu_16 + 1_sui8 == std::numeric_limits<sui16>::max());// overflow
static_assert(maxu_32 + 1_sui8 == std::numeric_limits<sui32>::max());// overflow
static_assert(maxu_64 + 1_sui8 == std::numeric_limits<sui64>::max());// overflow
check_does_compile(not ,  sui8,  +   1_ssi8  +) // mixing signedness
check_does_compile(not ,  sui16, +   1_ssi8  +) // mixing signedness
check_does_compile(not ,  sui32, +   1_ssi8  +) // mixing signedness
check_does_compile(not ,  sui64, +   1_ssi8  +) // mixing signedness
check_does_compile(    ,  sui8,  +  1_sui8  + 1_sui8  +) // same signedness
check_does_compile(    ,  sui16, +  1_sui16 + 1_sui8  +) // same signedness
check_does_compile(    ,  sui32, +  1_sui32 + 1_sui8  +) // same signedness
check_does_compile(    ,  sui64, +  1_sui64 + 1_sui8  +) // same signedness
// check substract:
static_assert(max_8 == max_8 - -1_ssi8); // overflow
static_assert(max_16 == max_16 - -1_ssi8); // overflow
static_assert(max_32 == max_32 - -1_ssi8); // overflow
static_assert(max_64 == max_64 - -1_ssi8); // overflow
check_does_compile(    ,  ssi8,  +  max_8  - 1_ssi8  +) //
check_does_compile(    ,  ssi16, +  max_16 - 1_ssi8  +) //
check_does_compile(    ,  ssi32, +  max_32 - 1_ssi8  +) //
check_does_compile(    ,  ssi64, +  max_64 - 1_ssi8  +) //
static_assert(min_8 == min_8 - 1_ssi8);
static_assert(min_16 == min_16 - 1_ssi8);
static_assert(min_32 == min_32 - 1_ssi8);
static_assert(min_64 == min_64 - 1_ssi8);
check_does_compile(not ,  ssi8,   - 1_sui8  +) // mixing signedness
check_does_compile(not ,  ssi16,  - 1_sui8  +) // mixing signedness
check_does_compile(not ,  ssi32,  - 1_sui8  +) // mixing signedness
check_does_compile(not ,  ssi64,  - 1_sui8  +) // mixing signedness
static_assert(0_sui8 == 0_sui8 - 1_sui8);
static_assert(0_sui16 == 0_sui16 - 1_sui8);
static_assert(0_sui32 == 0_sui32 - 1_sui8);
static_assert(0_sui64 == 0_sui64 - 1_sui8);
check_does_compile(not ,  sui8,   - 1_ssi8  +) // mixing signedness
check_does_compile(not ,  sui16,  - 1_ssi8  +) // mixing signedness
check_does_compile(not ,  sui32,  - 1_ssi8  +) // mixing signedness
check_does_compile(not ,  sui64,  - 1_ssi8  +) // mixing signedness
check_does_compile(    ,  sui8,  +  1_sui8  - 1_sui8  -) // same signedness
check_does_compile(    ,  sui16, +  1_sui16 - 1_sui8  +) // same signedness
check_does_compile(    ,  sui32, +  1_sui32 - 1_sui8  +) // same signedness
check_does_compile(    ,  sui64, +  1_sui64 - 1_sui8  +) // same signedness


}
#undef check_does_compile
#undef concat_line_impl
#undef concat_line
