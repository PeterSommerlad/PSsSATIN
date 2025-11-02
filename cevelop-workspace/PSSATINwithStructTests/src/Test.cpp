#include "satins.h"
#include "cute.h"
#include "ide_listener.h"
#include "xml_listener.h"
#include "cute_runner.h"
#include "TestForZeroReturnAssertWithNDEBUG.h"
#include "CodeGenBenchmark.h"
#include "OverflowCheckedTests.h"
#include "NonBuiltInOverflowDetectionTests.h"
#include <type_traits>
#include <cstddef>



using namespace satins::literals;

static_assert(0x8000_sui16 == 32768_sui16);
static_assert(0x7fff_sui32 < 100000_sui32);
static_assert(std::is_same_v<std::uint64_t,satins::ULT<satins::sui64>>);
namespace _testing {


template<typename FROM, typename=void>
constexpr bool
from_int_compiles=false;

template<typename FROM>
constexpr bool
from_int_compiles<FROM,std::void_t<decltype(satins::from_int(FROM{}))>> = true;

static_assert(from_int_compiles<unsigned char>);
static_assert(from_int_compiles<signed char>);
static_assert(from_int_compiles<short>);
static_assert(from_int_compiles<unsigned short>);
static_assert(from_int_compiles<int>);
static_assert(from_int_compiles<unsigned>);
static_assert(from_int_compiles<long>);
static_assert(from_int_compiles<unsigned long>);
static_assert(from_int_compiles<long long>);
static_assert(from_int_compiles<unsigned long long>);
static_assert(from_int_compiles<std::uint8_t>);
static_assert(from_int_compiles<std::uint16_t>);
static_assert(from_int_compiles<std::uint32_t>);
static_assert(from_int_compiles<std::uint64_t>);
static_assert(from_int_compiles<std::int8_t>);
static_assert(from_int_compiles<std::int16_t>);
static_assert(from_int_compiles<std::int32_t>);
static_assert(from_int_compiles<std::int64_t>);


static_assert(! from_int_compiles<bool>);
static_assert(! from_int_compiles<char>);
#ifdef __cpp_char8_t
static_assert(! from_int_compiles<char8_t>);
#endif
static_assert(! from_int_compiles<wchar_t>);
static_assert(! from_int_compiles<char16_t>);
static_assert(! from_int_compiles<char32_t>);

using namespace satins;
static_assert(1_sui8 == from_int(uint8_t(1)));
static_assert(42_ssi8 == from_int_to<ssi8>(42));
//static_assert(32_sui8 == from_int(' ')); // does not compile see above
//static_assert(32_sui8 == from_int(u' ')); // does not compile
//static_assert(32_sui8 == from_int(U' ')); // does not compile
//static_assert(32_sui8 == from_int(L' ')); // does not compile
//static_assert(1_sui8 == from_int_to<sui8>(true)); // does not compile

static_assert(42_ssi64 == from_int(42L));
static_assert(42_ssi64 == from_int(42LL));
static_assert(42_ssi32 == from_int(42));
static_assert(42_sui64 == from_int(42uL));
static_assert(42_sui64 == from_int(42uLL));
static_assert(42_sui32 == from_int(42u));


static_assert(detail_::is_saturatingint_v<sui8>);
static_assert(detail_::is_saturatingint_v<sui16>);
static_assert(detail_::is_saturatingint_v<sui32>);
static_assert(detail_::is_saturatingint_v<sui64>);
static_assert(detail_::is_saturatingint_v<ssi8>);
static_assert(detail_::is_saturatingint_v<ssi16>);
static_assert(detail_::is_saturatingint_v<ssi32>);
static_assert(detail_::is_saturatingint_v<ssi64>);
enum class enum4test{};
static_assert(!detail_::is_saturatingint_v<enum4test>);
static_assert(!detail_::is_saturatingint_v<std::byte>);
static_assert(!detail_::is_saturatingint_v<int>);
using detail_::promote_keep_signedness;
static_assert(std::is_same_v<unsigned,decltype(promote_keep_signedness(1_sui8)+1)>);
static_assert(std::is_same_v<unsigned,decltype(promote_keep_signedness(2_sui16)+1)>);
static_assert(std::is_same_v<int,decltype(promote_keep_signedness(1_ssi8))>);
static_assert(std::is_same_v<int,decltype(promote_keep_signedness(2_ssi16))>);
static_assert(std::is_same_v<uint8_t,ULT<sui8>>);
static_assert(std::is_same_v<uint16_t,ULT<sui16>>);

// Test case from Frank Buergel (extended):
// missing sign extension from 32 to 64bit
// discovered further bugs wrt signed division
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
//constexpr auto max_64 { std::numeric_limits<ssi64>::max() };

//constexpr auto maxu_8  { std::numeric_limits<sui8 >::max() };
//constexpr auto maxu_16 { std::numeric_limits<sui16>::max() };
//constexpr auto maxu_32 { std::numeric_limits<sui32>::max() };
//constexpr auto maxu_64 { std::numeric_limits<sui64>::max() };
//constexpr auto v1u_64 {  1_sui64 };
//constexpr auto v1u_32 {  1_sui32 };
//constexpr auto v1u_16 {  1_sui16 };
constexpr auto v1u_8  {  1_sui8 };
//constexpr auto v2u_64 {  2_sui64 };
//constexpr auto v2u_32 {  2_sui32 };
//constexpr auto v2u_16 {  2_sui16 };
//constexpr auto v2u_8  {  2_sui8 };
}

static_assert(v1_64 + vminus1_64 == 0_ssi64 );
static_assert(v1_64 + vminus1_32 == 0_ssi64 );
static_assert(v1_64 + vminus1_16 == 0_ssi64 );
static_assert(v1_64 + vminus1_8  == 0_ssi64 );
static_assert(v1_32 + vminus1_64 == 0_ssi64 );
static_assert(v1_32 + vminus1_32 == 0_ssi32 );
static_assert(v1_32 + vminus1_16 == 0_ssi32 );
static_assert(v1_32 + vminus1_8  == 0_ssi32 );
static_assert(v1_16 + vminus1_64 == 0_ssi64 );
static_assert(v1_16 + vminus1_32 == 0_ssi32 );
static_assert(v1_16 + vminus1_16 == 0_ssi16 );
static_assert(v1_16 + vminus1_8  == 0_ssi16 );
static_assert(v1_8  + vminus1_64 == 0_ssi64 );
static_assert(v1_8  + vminus1_32 == 0_ssi32 );
static_assert(v1_8  + vminus1_16 == 0_ssi16 );
static_assert(v1_8  + vminus1_8  == 0_ssi8 );

static_assert(vminus1_64 +  v1_64== 0_ssi64 );
static_assert(vminus1_32 +  v1_64== 0_ssi64 );
static_assert(vminus1_16 +  v1_64== 0_ssi64 );
static_assert(vminus1_8  +  v1_64== 0_ssi64 );
static_assert(vminus1_64 +  v1_32== 0_ssi64 );
static_assert(vminus1_32 +  v1_32== 0_ssi32 );
static_assert(vminus1_16 +  v1_32== 0_ssi32 );
static_assert(vminus1_8  +  v1_32== 0_ssi32 );
static_assert(vminus1_64 +  v1_16== 0_ssi64 );
static_assert(vminus1_32 +  v1_16== 0_ssi32 );
static_assert(vminus1_16 +  v1_16== 0_ssi16 );
static_assert(vminus1_8  +  v1_16== 0_ssi16 );
static_assert(vminus1_64 +  v1_8 == 0_ssi64 );
static_assert(vminus1_32 +  v1_8 == 0_ssi32 );
static_assert(vminus1_16 +  v1_8 == 0_ssi16 );
static_assert(vminus1_8  +  v1_8 == 0_ssi8 );


// demonstrate wrapping:
//static_assert(max_64 + v1_64 == min_64 );
//static_assert(max_64 + v1_32 == min_64 );
//static_assert(max_64 + v1_16 == min_64 );
//static_assert(max_64 + v1_8  == min_64 );
static_assert(max_32 + v1_64 == 0x8000'0000_ssi64 );
//static_assert(max_32 + v1_32 == min_32 );
//static_assert(max_32 + v1_16 == min_32 );
//static_assert(max_32 + v1_8  == min_32 );
static_assert(max_16 + v1_64 == 0x8000_ssi64 );
static_assert(max_16 + v1_32 == 0x8000_ssi32 );
//static_assert(max_16 + v1_16 == min_16 );
//static_assert(max_16 + v1_8  == min_16 );
static_assert(max_8  + v1_64 == 128_ssi64 );
static_assert(max_8  + v1_32 == 128_ssi32 );
static_assert(max_8  + v1_16 == 128_ssi16 );
//static_assert(max_8  + v1_8  == min_8 );


static_assert(v1_64 - vminus1_64 == 2_ssi64 );
static_assert(v1_64 - vminus1_32 == 2_ssi64 );
static_assert(v1_64 - vminus1_16 == 2_ssi64 );
static_assert(v1_64 - vminus1_8  == 2_ssi64 );
static_assert(v1_32 - vminus1_64 == 2_ssi64 );
static_assert(v1_32 - vminus1_32 == 2_ssi32 );
static_assert(v1_32 - vminus1_16 == 2_ssi32 );
static_assert(v1_32 - vminus1_8  == 2_ssi32 );
static_assert(v1_16 - vminus1_64 == 2_ssi64 );
static_assert(v1_16 - vminus1_32 == 2_ssi32 );
static_assert(v1_16 - vminus1_16 == 2_ssi16 );
static_assert(v1_16 - vminus1_8  == 2_ssi16 );
static_assert(v1_8  - vminus1_64 == 2_ssi64 );
static_assert(v1_8  - vminus1_32 == 2_ssi32 );
static_assert(v1_8  - vminus1_16 == 2_ssi16 );
static_assert(v1_8  - vminus1_8  == 2_ssi8 );


static_assert(vminus1_64 - v1_64  == -2_ssi64 );
static_assert(vminus1_32 - v1_64  == -2_ssi64 );
static_assert(vminus1_16 - v1_64  == -2_ssi64 );
static_assert(vminus1_8  - v1_64  == -2_ssi64 );
static_assert(vminus1_64 - v1_32  == -2_ssi64 );
static_assert(vminus1_32 - v1_32  == -2_ssi32 );
static_assert(vminus1_16 - v1_32  == -2_ssi32 );
static_assert(vminus1_8  - v1_32  == -2_ssi32 );
static_assert(vminus1_64 - v1_16  == -2_ssi64 );
static_assert(vminus1_32 - v1_16  == -2_ssi32 );
static_assert(vminus1_16 - v1_16  == -2_ssi16 );
static_assert(vminus1_8  - v1_16  == -2_ssi16 );
static_assert(vminus1_64 - v1_8   == -2_ssi64 );
static_assert(vminus1_32 - v1_8   == -2_ssi32 );
static_assert(vminus1_16 - v1_8   == -2_ssi16 );
static_assert(vminus1_8  - v1_8   == -2_ssi8 );

static_assert(v1_64 * vminus1_64 == -1_ssi64 );
static_assert(v1_64 * vminus1_32 == -1_ssi64 );
static_assert(v1_64 * vminus1_16 == -1_ssi64 );
static_assert(v1_64 * vminus1_8  == -1_ssi64 );
static_assert(v1_32 * vminus1_64 == -1_ssi64 );
static_assert(v1_32 * vminus1_32 == -1_ssi32 );
static_assert(v1_32 * vminus1_16 == -1_ssi32 );
static_assert(v1_32 * vminus1_8  == -1_ssi32 );
static_assert(v1_16 * vminus1_64 == -1_ssi64 );
static_assert(v1_16 * vminus1_32 == -1_ssi32 );
static_assert(v1_16 * vminus1_16 == -1_ssi16 );
static_assert(v1_16 * vminus1_8  == -1_ssi16 );
static_assert(v1_8  * vminus1_64 == -1_ssi64 );
static_assert(v1_8  * vminus1_32 == -1_ssi32 );
static_assert(v1_8  * vminus1_16 == -1_ssi16 );
static_assert(v1_8  * vminus1_8  == -1_ssi8 );

static_assert(vminus1_64 *  v1_64== -1_ssi64 );
static_assert(vminus1_32 *  v1_64== -1_ssi64 );
static_assert(vminus1_16 *  v1_64== -1_ssi64 );
static_assert(vminus1_8  *  v1_64== -1_ssi64 );
static_assert(vminus1_64 *  v1_32== -1_ssi64 );
static_assert(vminus1_32 *  v1_32== -1_ssi32 );
static_assert(vminus1_16 *  v1_32== -1_ssi32 );
static_assert(vminus1_8  *  v1_32== -1_ssi32 );
static_assert(vminus1_64 *  v1_16== -1_ssi64 );
static_assert(vminus1_32 *  v1_16== -1_ssi32 );
static_assert(vminus1_16 *  v1_16== -1_ssi16 );
static_assert(vminus1_8  *  v1_16== -1_ssi16 );
static_assert(vminus1_64 *  v1_8 == -1_ssi64 );
static_assert(vminus1_32 *  v1_8 == -1_ssi32 );
static_assert(vminus1_16 *  v1_8 == -1_ssi16 );
static_assert(vminus1_8  *  v1_8 == -1_ssi8 );

static_assert(vminus1_64 *  v2_64== -2_ssi64 );
static_assert(vminus1_32 *  v2_64== -2_ssi64 );
static_assert(vminus1_16 *  v2_64== -2_ssi64 );
static_assert(vminus1_8  *  v2_64== -2_ssi64 );
static_assert(vminus1_64 *  v2_32== -2_ssi64 );
static_assert(vminus1_32 *  v2_32== -2_ssi32 );
static_assert(vminus1_16 *  v2_32== -2_ssi32 );
static_assert(vminus1_8  *  v2_32== -2_ssi32 );
static_assert(vminus1_64 *  v2_16== -2_ssi64 );
static_assert(vminus1_32 *  v2_16== -2_ssi32 );
static_assert(vminus1_16 *  v2_16== -2_ssi16 );
static_assert(vminus1_8  *  v2_16== -2_ssi16 );
static_assert(vminus1_64 *  v2_8 == -2_ssi64 );
static_assert(vminus1_32 *  v2_8 == -2_ssi32 );
static_assert(vminus1_16 *  v2_8 == -2_ssi16 );
static_assert(vminus1_8  *  v2_8 == -2_ssi8 );

static_assert(vminus1_64 * -v2_64==  2_ssi64 );
static_assert(vminus1_32 * -v2_64==  2_ssi64 );
static_assert(vminus1_16 * -v2_64==  2_ssi64 );
static_assert(vminus1_8  * -v2_64==  2_ssi64 );
static_assert(vminus1_64 * -v2_32==  2_ssi64 );
static_assert(vminus1_32 * -v2_32==  2_ssi32 );
static_assert(vminus1_16 * -v2_32==  2_ssi32 );
static_assert(vminus1_8  * -v2_32==  2_ssi32 );
static_assert(vminus1_64 * -v2_16==  2_ssi64 );
static_assert(vminus1_32 * -v2_16==  2_ssi32 );
static_assert(vminus1_16 * -v2_16==  2_ssi16 );
static_assert(vminus1_8  * -v2_16==  2_ssi16 );
static_assert(vminus1_64 * -v2_8 ==  2_ssi64 );
static_assert(vminus1_32 * -v2_8 ==  2_ssi32 );
static_assert(vminus1_16 * -v2_8 ==  2_ssi16 );
static_assert(vminus1_8  * -v2_8 ==  2_ssi8 );

static_assert(v2_64 * -v2_64== -4_ssi64 );
static_assert(v2_32 * -v2_64== -4_ssi64 );
static_assert(v2_16 * -v2_64== -4_ssi64 );
static_assert(v2_8  * -v2_64== -4_ssi64 );
static_assert(v2_64 * -v2_32== -4_ssi64 );
static_assert(v2_32 * -v2_32== -4_ssi32 );
static_assert(v2_16 * -v2_32== -4_ssi32 );
static_assert(v2_8  * -v2_32== -4_ssi32 );
static_assert(v2_64 * -v2_16== -4_ssi64 );
static_assert(v2_32 * -v2_16== -4_ssi32 );
static_assert(v2_16 * -v2_16== -4_ssi16 );
static_assert(v2_8  * -v2_16== -4_ssi16 );
static_assert(v2_64 * -v2_8 == -4_ssi64 );
static_assert(v2_32 * -v2_8 == -4_ssi32 );
static_assert(v2_16 * -v2_8 == -4_ssi16 );
static_assert(v2_8  * -v2_8 == -4_ssi8 );

static_assert(-v2_64 * -v2_64== 4_ssi64 );
static_assert(-v2_32 * -v2_64== 4_ssi64 );
static_assert(-v2_16 * -v2_64== 4_ssi64 );
static_assert(-v2_8  * -v2_64== 4_ssi64 );
static_assert(-v2_64 * -v2_32== 4_ssi64 );
static_assert(-v2_32 * -v2_32== 4_ssi32 );
static_assert(-v2_16 * -v2_32== 4_ssi32 );
static_assert(-v2_8  * -v2_32== 4_ssi32 );
static_assert(-v2_64 * -v2_16== 4_ssi64 );
static_assert(-v2_32 * -v2_16== 4_ssi32 );
static_assert(-v2_16 * -v2_16== 4_ssi16 );
static_assert(-v2_8  * -v2_16== 4_ssi16 );
static_assert(-v2_64 * -v2_8 == 4_ssi64 );
static_assert(-v2_32 * -v2_8 == 4_ssi32 );
static_assert(-v2_16 * -v2_8 == 4_ssi16 );
static_assert(-v2_8  * -v2_8 == 4_ssi8 );

// demonstrate no wrapping: (see below)
//static_assert(min_64 * v2_64 == 0_ssi64 );
//static_assert(min_64 * v2_32 == 0_ssi64  );
//static_assert(min_64 * v2_16 == 0_ssi64  );
//static_assert(min_64 * v2_8  == 0_ssi64 );
static_assert(min_32 * v2_64 == -0x1'0000'0000_ssi64 );
//static_assert(min_32 * v2_32 == 0_ssi32 );
//static_assert(min_32 * v2_16 == 0_ssi32 );
//static_assert(min_32 * v2_8  == 0_ssi32 );
static_assert(min_16 * v2_64 == -0x1'0000_ssi64  );
static_assert(min_16 * v2_32 == -0x1'0000_ssi32 );
//static_assert(min_16 * v2_16 == 0_ssi16 );
//static_assert(min_16 * v2_8  == 0_ssi16 );
static_assert(min_8  * v2_64 == -256_ssi64 );
static_assert(min_8  * v2_32 == -256_ssi32 );
static_assert(min_8  * v2_16 == -256_ssi16 );
//static_assert(min_8  * v2_8  == 0_ssi8 );


static_assert(v1_64 / vminus1_64 == -1_ssi64 );
static_assert(v1_64 / vminus1_32 == -1_ssi64 );
static_assert(v1_64 / vminus1_16 == -1_ssi64 );
static_assert(v1_64 / vminus1_8  == -1_ssi64 );
static_assert(v1_32 / vminus1_64 == -1_ssi64 );
static_assert(v1_32 / vminus1_32 == -1_ssi32 );
static_assert(v1_32 / vminus1_16 == -1_ssi32 );
static_assert(v1_32 / vminus1_8  == -1_ssi32 );
static_assert(v1_16 / vminus1_64 == -1_ssi64 );
static_assert(v1_16 / vminus1_32 == -1_ssi32 );
static_assert(v1_16 / vminus1_16 == -1_ssi16 );
static_assert(v1_16 / vminus1_8  == -1_ssi16 );
static_assert(v1_8  / vminus1_64 == -1_ssi64 );
static_assert(v1_8  / vminus1_32 == -1_ssi32 );
static_assert(v1_8  / vminus1_16 == -1_ssi16 );
static_assert(v1_8  / vminus1_8  == -1_ssi8 );

static_assert(vminus1_64 /  v1_64== -1_ssi64 );
static_assert(vminus1_32 /  v1_64== -1_ssi64 );
static_assert(vminus1_16 /  v1_64== -1_ssi64 );
static_assert(vminus1_8  /  v1_64== -1_ssi64 );
static_assert(vminus1_64 /  v1_32== -1_ssi64 );
static_assert(vminus1_32 /  v1_32== -1_ssi32 );
static_assert(vminus1_16 /  v1_32== -1_ssi32 );
static_assert(vminus1_8  /  v1_32== -1_ssi32 );
static_assert(vminus1_64 /  v1_16== -1_ssi64 );
static_assert(vminus1_32 /  v1_16== -1_ssi32 );
static_assert(vminus1_16 /  v1_16== -1_ssi16 );
static_assert(vminus1_8  /  v1_16== -1_ssi16 );
static_assert(vminus1_64 /  v1_8 == -1_ssi64 );
static_assert(vminus1_32 /  v1_8 == -1_ssi32 );
static_assert(vminus1_16 /  v1_8 == -1_ssi16 );
static_assert(vminus1_8  /  v1_8 == -1_ssi8 );


static_assert(v2_64 / vminus1_64 == -2_ssi64 );
static_assert(v2_64 / vminus1_32 == -2_ssi64 );
static_assert(v2_64 / vminus1_16 == -2_ssi64 );
static_assert(v2_64 / vminus1_8  == -2_ssi64 );
static_assert(v2_32 / vminus1_64 == -2_ssi64 );
static_assert(v2_32 / vminus1_32 == -2_ssi32 );
static_assert(v2_32 / vminus1_16 == -2_ssi32 );
static_assert(v2_32 / vminus1_8  == -2_ssi32 );
static_assert(v2_16 / vminus1_64 == -2_ssi64 );
static_assert(v2_16 / vminus1_32 == -2_ssi32 );
static_assert(v2_16 / vminus1_16 == -2_ssi16 );
static_assert(v2_16 / vminus1_8  == -2_ssi16 );
static_assert(v2_8  / vminus1_64 == -2_ssi64 );
static_assert(v2_8  / vminus1_32 == -2_ssi32 );
static_assert(v2_8  / vminus1_16 == -2_ssi16 );
static_assert(v2_8  / vminus1_8  == -2_ssi8 );

static_assert(vminus1_64 /  v2_64== 0_ssi64 );
static_assert(vminus1_32 /  v2_64== 0_ssi64 );
static_assert(vminus1_16 /  v2_64== 0_ssi64 );
static_assert(vminus1_8  /  v2_64== 0_ssi64 );
static_assert(vminus1_64 /  v2_32== 0_ssi64 );
static_assert(vminus1_32 /  v2_32== 0_ssi32 );
static_assert(vminus1_16 /  v2_32== 0_ssi32 );
static_assert(vminus1_8  /  v2_32== 0_ssi32 );
static_assert(vminus1_64 /  v2_16== 0_ssi64 );
static_assert(vminus1_32 /  v2_16== 0_ssi32 );
static_assert(vminus1_16 /  v2_16== 0_ssi16 );
static_assert(vminus1_8  /  v2_16== 0_ssi16 );
static_assert(vminus1_64 /  v2_8 == 0_ssi64 );
static_assert(vminus1_32 /  v2_8 == 0_ssi32 );
static_assert(vminus1_16 /  v2_8 == 0_ssi16 );
static_assert(vminus1_8  /  v2_8 == 0_ssi8 );

static_assert(min_64 /  min_64 ==  1_ssi64 );
static_assert(min_32 /  min_64 ==  0_ssi64 );
static_assert(min_16 /  min_64 ==  0_ssi64 );
static_assert(min_8  /  min_64 ==  0_ssi64 );
static_assert(min_64 /  min_32 ==  0x1'0000'0000_ssi64 );
static_assert(min_32 /  min_32 ==  1_ssi32 );
static_assert(min_16 /  min_32 ==  0_ssi32 );
static_assert(min_8  /  min_32 ==  0_ssi32 );
static_assert(min_64 /  min_16 ==  0x1'0000'0000'0000_ssi64 );
static_assert(min_32 /  min_16 ==  0x1'0000_ssi32 );
static_assert(min_16 /  min_16 ==  1_ssi16 );
static_assert(min_8  /  min_16 ==  0_ssi16 );
static_assert(min_64 /  min_8  ==  0x100'0000'0000'0000_ssi64 );
static_assert(min_32 /  min_8  ==  0x100'0000_ssi32 );
static_assert(min_16 /  min_8  ==  256_ssi16 );
static_assert(min_8  /  min_8  ==  1_ssi8 );

// demonstrate wrapping: (see below with overflow detection)
//static_assert(min_64 / vminus1_64 == min_64 );
//static_assert(min_64 / vminus1_32 == min_64 );
//static_assert(min_64 / vminus1_16 == min_64 );
//static_assert(min_64 / vminus1_8  == min_64 );
//static_assert(min_32 / vminus1_64 == 0x8000'0000_ssi64 );
//static_assert(min_32 / vminus1_32 == min_32 );
//static_assert(min_32 / vminus1_16 == min_32 );
//static_assert(min_32 / vminus1_8  == min_32 );
//static_assert(min_16 / vminus1_64 == -static_cast<si64>(min_16)  );
//static_assert(min_16 / vminus1_32 == 0x8000_ssi32 );
//static_assert(min_16 / vminus1_16 == min_16 );
//static_assert(min_16 / vminus1_8  == min_16 );
//static_assert(min_8  / vminus1_64 == 128_ssi64 );
//static_assert(min_8  / vminus1_32 == 128_ssi32 );
//static_assert(min_8  / vminus1_16 == 128_ssi16 );
//static_assert(min_8  / vminus1_8  == min_8 );

static_assert(min_64 / v1_64 == min_64 );
static_assert(min_64 / v1_32 == min_64 );
static_assert(min_64 / v1_16 == min_64 );
static_assert(min_64 / v1_8  == min_64 );
static_assert(min_32 / v1_64 == -0x8000'0000_ssi64 );
static_assert(min_32 / v1_32 == min_32 );
static_assert(min_32 / v1_16 == min_32 );
static_assert(min_32 / v1_8  == min_32 );
static_assert(min_16 / v1_64 == -0x8000_ssi64  );
static_assert(min_16 / v1_32 == -0x8000_ssi32 );
static_assert(min_16 / v1_16 == min_16 );
static_assert(min_16 / v1_8  == min_16 );
static_assert(min_8  / v1_64 == -128_ssi64 );
static_assert(min_8  / v1_32 == -128_ssi32 );
static_assert(min_8  / v1_16 == -128_ssi16 );
static_assert(min_8  / v1_8  == min_8 );

static_assert(min_64 / v2_64 == -0x4000'0000'0000'0000_ssi64 );
static_assert(min_64 / v2_32 == -0x4000'0000'0000'0000_ssi64  );
static_assert(min_64 / v2_16 == -0x4000'0000'0000'0000_ssi64  );
static_assert(min_64 / v2_8  == -0x4000'0000'0000'0000_ssi64 );
static_assert(min_32 / v2_64 == -0x4000'0000_ssi64 );
static_assert(min_32 / v2_32 == -0x4000'0000_ssi32 );
static_assert(min_32 / v2_16 == -0x4000'0000_ssi32 );
static_assert(min_32 / v2_8  == -0x4000'0000_ssi32 );
static_assert(min_16 / v2_64 == -0x4000_ssi64  );
static_assert(min_16 / v2_32 == -0x4000_ssi32 );
static_assert(min_16 / v2_16 == -0x4000_ssi16 );
static_assert(min_16 / v2_8  == -0x4000_ssi16 );
static_assert(min_8  / v2_64 == -64_ssi64 );
static_assert(min_8  / v2_32 == -64_ssi32 );
static_assert(min_8  / v2_16 == -64_ssi16 );
static_assert(min_8  / v2_8  == -64_ssi8 );


static_assert(-100_ssi32 / -9_ssi64 == 11_ssi64);
static_assert(100_ssi32 / 9_ssi64 == 11_ssi64);
static_assert(-100_ssi32 / 9_ssi64 == -11_ssi64);
static_assert(100_ssi32 / -9_ssi64 == -11_ssi64);

static_assert((v1u_8 << v1u_8) == 2_sui8);

//static_assert(std::numeric_limits<si32>::min() / 1_ssi32 == std::numeric_limits<si32>::min()); // wraps
//static_assert(std::numeric_limits<si32>::min() / -1_ssi32 == std::numeric_limits<si32>::min()); // wraps



template<typename T, typename WHAT>
constexpr bool
isa = std::is_same_v<std::remove_cvref_t<T>,WHAT>;


template<typename T>
constexpr bool
is_unsigned = isa<T, unsigned char>
|| isa<T, unsigned short>
|| isa<T, unsigned >
|| isa<T, unsigned long>
|| isa<T, unsigned long long>;

template<typename T>
constexpr bool
is_signed = isa<T, signed char>
|| isa<T, short>
|| isa<T, int >
|| isa<T, long>
|| isa<T, long long>;


template<typename T>
constexpr bool
is_integer = is_unsigned<T>||is_signed<T>;

static_assert(is_integer<ULT<sui16>>);
static_assert(!is_integer<sui16>);
static_assert(is_integer<ULT<ssi16>>);
static_assert(!is_integer<ssi16>);


void signedIntegerBoundaryTestResultRecovery(){
    // temporary testcase for getting static_asserts above right
    ASSERT_EQUAL(0x8000'0000_ssi64, max_32 + v1_64  );
}


}

void si8preincrement(){
    auto one = 1_ssi8;
    ASSERT_EQUAL(2_ssi8,++one);
}
void si8postincrement(){
    auto one = 1_ssi8;
    ASSERT_EQUAL(1_ssi8,one++);
    ASSERT_EQUAL(2_ssi8,one);
}
void si8predecrement(){
    auto one = 1_ssi8;
    ASSERT_EQUAL(0_ssi8,--one);
}
void si8postdecrement(){
    auto one = 1_ssi8;
    ASSERT_EQUAL(1_ssi8,one--);
    ASSERT_EQUAL(0_ssi8,one);
}
void si16preincrement(){
    auto one = 1_ssi16;
    ASSERT_EQUAL(2_ssi16,++one);
}
void si16postincrement(){
    auto one = 1_ssi16;
    ASSERT_EQUAL(1_ssi16,one++);
    ASSERT_EQUAL(2_ssi16,one);
}
void si16predecrement(){
    auto one = 1_ssi16;
    ASSERT_EQUAL(0_ssi16,--one);
}
void si16postdecrement(){
    auto one = 1_ssi16;
    ASSERT_EQUAL(1_ssi16,one--);
    ASSERT_EQUAL(0_ssi16,one);
}
void si32preincrement(){
    auto one = 1_ssi32;
    ASSERT_EQUAL(2_ssi32,++one);
}
void si32postincrement(){
    auto one = 1_ssi32;
    ASSERT_EQUAL(1_ssi32,one++);
    ASSERT_EQUAL(2_ssi32,one);
}
void si32predecrement(){
    auto one = 1_ssi32;
    ASSERT_EQUAL(0_ssi32,--one);
}
void si32postdecrement(){
    auto one = 1_ssi32;
    ASSERT_EQUAL(1_ssi32,one--);
    ASSERT_EQUAL(0_ssi32,one);
}
void si64preincrement(){
    auto one = 1_ssi64;
    ASSERT_EQUAL(2_ssi64,++one);
}
void si64postincrement(){
    auto one = 1_ssi64;
    ASSERT_EQUAL(1_ssi64,one++);
    ASSERT_EQUAL(2_ssi64,one);
}
void si64predecrement(){
    auto one = 1_ssi64;
    ASSERT_EQUAL(0_ssi64,--one);
}
void si64postdecrement(){
    auto one = 1_ssi64;
    ASSERT_EQUAL(1_ssi64,one--);
    ASSERT_EQUAL(0_ssi64,one);
}


void ui8preincrement(){
    auto one = 1_sui8;
    ASSERT_EQUAL(2_sui8,++one);
}
void ui8postincrement(){
    auto one = 1_sui8;
    ASSERT_EQUAL(1_sui8,one++);
    ASSERT_EQUAL(2_sui8,one);
}
void ui8predecrement(){
    auto one = 1_sui8;
    ASSERT_EQUAL(0_sui8,--one);
}
void ui8postdecrement(){
    auto one = 1_sui8;
    ASSERT_EQUAL(1_sui8,one--);
    ASSERT_EQUAL(0_sui8,one);
}
void ui16preincrement(){
    auto one = 1_sui16;
    ASSERT_EQUAL(2_sui16,++one);
}
void ui16postincrement(){
    auto one = 1_sui16;
    ASSERT_EQUAL(1_sui16,one++);
    ASSERT_EQUAL(2_sui16,one);
}
void ui16predecrement(){
    auto one = 1_sui16;
    ASSERT_EQUAL(0_sui16,--one);
}
void ui16postdecrement(){
    auto one = 1_sui16;
    ASSERT_EQUAL(1_sui16,one--);
    ASSERT_EQUAL(0_sui16,one);
}
void ui32preincrement(){
    auto one = 1_sui32;
    ASSERT_EQUAL(2_sui32,++one);
}
void ui32postincrement(){
    auto one = 1_sui32;
    ASSERT_EQUAL(1_sui32,one++);
    ASSERT_EQUAL(2_sui32,one);
}
void ui32predecrement(){
    auto one = 1_sui32;
    ASSERT_EQUAL(0_sui32,--one);
}
void ui32postdecrement(){
    auto one = 1_sui32;
    ASSERT_EQUAL(1_sui32,one--);
    ASSERT_EQUAL(0_sui32,one);
}
void ui64preincrement(){
    auto one = 1_sui64;
    ASSERT_EQUAL(2_sui64,++one);
}
void ui64postincrement(){
    auto one = 1_sui64;
    ASSERT_EQUAL(1_sui64,one++);
    ASSERT_EQUAL(2_sui64,one);
}
void ui64predecrement(){
    auto one = 1_sui64;
    ASSERT_EQUAL(0_sui64,--one);
}
void ui64postdecrement(){
    auto one = 1_sui64;
    ASSERT_EQUAL(1_sui64,one--);
    ASSERT_EQUAL(0_sui64,one);
}
void ui16intExists() {
    using satins::sui16;
    auto large=0xff00_sui16;
    //0x10000_sui16; // compile error
    //ui16{0xfffff}; // narrowing detection
    ASSERT_EQUAL(sui16{0xff00u},large);
}

void ui16NotEqualAutomaticInCpp20(){
    ASSERT(0xf_sui16 != 0_sui16);
}

void ui16canbeadded(){
    ASSERT_EQUAL(100_sui16,75_sui16+25_sui16);
}

void ui16canbeaddedto(){
    auto l = 42_sui16;
    l += 8_sui8;
    //l += 1_sui32; // compile error
    ASSERT_EQUAL(50_sui16,l);
}

void ui16canbesubtracted(){
    ASSERT_EQUAL(50_sui16,75_sui16-25_sui16);
}

void ui16canbesubtractedfrom(){
    auto l = 42_sui16;
    l -= 8_sui8;
    //l -= 1_sui32; // compile error
    ASSERT_EQUAL(34_sui16,l);
}

void ui16canbemultiplied(){
    ASSERT_EQUAL(1875_sui16,75_sui16*25_sui16);
}

void ui16canbemultipliedwith(){
    auto l = 42_sui16;
    l *= 8_sui8;
    //l -= 1_sui32; // compile error
    ASSERT_EQUAL(336_sui16,l);
}


void ui16canbedivided(){
    ASSERT_EQUAL(3_sui16,75_sui16/25_sui16);
}

void ui16canbedividedby(){
    auto l = 42_sui16;
    l /= 7_sui8;
    //l /= 1_sui32; // compile error
    ASSERT_EQUAL(6_sui16,l);
}

void ui16canbemoduloed(){
    ASSERT_EQUAL(10_sui16,75_sui16%13_sui16);
}

void ui16canbemoduloedby(){
    auto l = 42_sui16;
    l %= 13_sui8;
    //l %= 1_sui32; // compile error
    ASSERT_EQUAL(3_sui16,l);
}


void ui16canbeanded(){
    ASSERT_EQUAL(0X0AA0_sui16,0x0ff0_sui16 & 0xAAAA_sui16);
}

void ui16canbeandedwith(){
    auto l = 0xff00_sui16;
    l &= 0xABCD_sui16;
    //l &= 1_sui8; // compile error
    ASSERT_EQUAL(0xAB00_sui16,l);
}

void ui16canbeored(){
    ASSERT_EQUAL(0XAFFA_sui16,0x0ff0_sui16 | 0xAAAA_sui16);
}

void ui16canbeoredwith(){
    auto l = 0xff00_sui16;
    l |= 0xABCD_sui16;
    //l |= 1_sui8; // compile error
    ASSERT_EQUAL(0xFFCD_sui16,l);
}

void ui16canbexored(){
    ASSERT_EQUAL(0XA55A_sui16,0x0ff0_sui16 ^ 0xAAAA_sui16);
}

void ui16canbexoredwith(){
    auto l = 0xff00_sui16;
    l ^= 0xABCD_sui16;
    //l ^= 1_sui8; // compile error
    ASSERT_EQUAL(0x54CD_sui16,l);
}

void ui16canbenegated(){
    ASSERT_EQUAL(0XA55A_sui16, ~0x5AA5_sui16 );
}

void ui16canbeleftshifted(){
    //constexpr auto l = 0x1_sui16 << 16_sui8; // compile error
    ASSERT_EQUAL(0XFF00_sui16,0x0ff0_sui16 << 0x4_sui8);
}

void ui16canbeleftshiftedby(){
    auto l = 0x00ff_sui16;
    l <<= 4_sui16;
    ASSERT_EQUAL(0x0FF0_sui16,l);
}
void ui16canberightshifted(){
    //constexpr auto l = 0x1_sui16 << 16_sui8; // compile error
    ASSERT_EQUAL(0X00FF_sui16,0x0ff0_sui16 >> 0x4_sui8);
}

void ui16canberightshiftedby(){
    auto l = 0x00ff_sui16;
    l >>= 4_sui16;
    ASSERT_EQUAL(0x0F_sui16,l);
}
void ui16canbepreincremented(){
    auto l = 0x00ff_sui16;
    ASSERT_EQUAL(0x100_sui16,++l);
    ASSERT_EQUAL(0x100_sui16,l);
}
void ui16canbepostincremented(){
    auto l = 0x00ff_sui16;
    ASSERT_EQUAL(0x00ff_sui16,l++);
    ASSERT_EQUAL(0x100_sui16,l);
}
void ui16canbepredecremented(){
    auto l = 0x00ff_sui16;
    ASSERT_EQUAL(0x100_sui16,++l);
    ASSERT_EQUAL(0x100_sui16,l);
}
void ui16canbepostdecremented(){
    auto l = 0x00ff_sui16;
    ASSERT_EQUAL(0x00ff_sui16,l++);
    ASSERT_EQUAL(0x100_sui16,l);
}

void ui16canbecompared(){
    auto l = 0x00ff_sui16;
    auto s = 0x000f_sui16;

	ASSERTM("check comparison", l != s && s < l && l >= s && !(l < s) && ! (l <= s));
}

void ui16canNotbecomparedwithui8() {
    auto l = 0x00ff_sui16;
    auto s = 0x000f_sui8;


//    ASSERTM("check comparison", l != s && s < l && l >= s && !(l < s) && ! (l <= s));

    auto ss = s + 0_sui16;
    ASSERTM("check comparison", l != ss && ss < l && l >= ss && !(l < ss) && ! (l <= ss));
}

void ui32CanNotbeComparedwithlong(){
    auto l = 0x00ff_sui32;
    auto s = std::uint32_t{0x000fU};


//    ASSERTM("check comparison", l != s && s < l && l >= s && !(l < s) && ! (l <= s));

    auto ss = satins::from_int(s);
    ASSERTM("check comparison", l != ss && ss < l && l >= ss && !(l < ss) && ! (l <= ss));

}


// signed test to check if result is correct (overflow wraps)

void si8canbeaddednormal(){
    ASSERT_EQUAL(42_ssi8, 21_ssi8 + 21_ssi8);
}

void si8Negation(){
    ASSERT_EQUAL(-1,satins::detail_::promote_keep_signedness(-1_ssi8));
}

void si8negationminintthrows(){
    ASSERT_EQUAL(std::numeric_limits<satins::ssi8>::max(),-(std::numeric_limits<satins::ssi8>::min()));
//    ASSERT_THROWS(std::ignore = -(std::numeric_limits<satins::ssi8>::min()), char const *);
}

void si8overflowIsDetected(){
    ASSERT_EQUAL(std::numeric_limits<satins::ssi8>::max(),127_ssi8+2_ssi8 );
    //ASSERT_THROWS(std::ignore = 127_ssi8+2_ssi8, char const *);
}

void si8subtraction(){
    ASSERT_EQUAL(-1_ssi8,2_ssi8-3_ssi8);
}

void si8subtractionoverflowdetected(){
    try {
    ASSERT_THROWS(std::ignore = ((-2_ssi8)-127_ssi8) , char const *);
    } catch(...){}
}
void si8multiplication(){
    ASSERT_EQUAL(120_ssi8 , 3_ssi8 * 40_ssi8);
}
void si8division(){
    ASSERT_EQUAL(3_ssi8 , 120_ssi8 / 40_ssi8);
}

void si8OutputAsInteger(){
    std::ostringstream out{};
    out << 42_ssi8;
    ASSERT_EQUAL("42",out.str());
}

void ui8OutputAsInteger(){
    std::ostringstream out{};
    out << 42_sui8;
    ASSERT_EQUAL("42",out.str());
}

void checkedFromInt(){
    using namespace satins;
    ASSERT_EQUAL(255_sui8,from_int_to<sui8>(2400u));
    //ASSERT_THROWS(std::ignore = from_int_to<sui8>(2400u), char const *);

}



namespace cppnowtalk{

void testUBforint() {
    std::ostringstream out{};
    out << 65535 * 32768 << '\n';
    // prints: 2147450880
#pragma GCC diagnostic push
#ifdef __clang__
    #pragma clang diagnostic ignored "-Winteger-overflow"
#else
    #pragma GCC diagnostic ignored "-Woverflow"
#endif
    out << 65536 * 32768 << '\n';
    //../src/Test.cpp:421:18: error: integer overflow in expression of type 'int' results in '-2147483648' [-Werror=overflow]
#pragma GCC diagnostic pop
    // prints: ?
    out << std::boolalpha << std::numeric_limits<int>::is_modulo << '\n';
    ASSERT_EQUAL("2147450880\n-2147483648\nfalse\n",out.str());
}
void testNoUBforunsigned() {
    std::ostringstream out{};
    out << 65535u * 32768u << '\n';
    // prints: 2147450880
    out << 65536u * 32768u << '\n';
    // prints: 2147483648

    out << 65536u * 32768u * 2u << '\n';
    // prints: ?
    out << std::boolalpha << std::numeric_limits<unsigned>::is_modulo << '\n';
    ASSERT_EQUAL("2147450880\n2147483648\n0\ntrue\n",out.str());
}
}

bool runAllTests(int argc, char const *argv[]) {
    cute::suite TestForZeroReturnAssertWithNDEBUG = make_suite_TestForZeroReturnAssertWithNDEBUG();
    TestForZeroReturnAssertWithNDEBUG.push_back(CUTE(cppnowtalk::testUBforint));
    TestForZeroReturnAssertWithNDEBUG.push_back(CUTE(cppnowtalk::testNoUBforunsigned));

    cute::suite s { };

    s.push_back(CUTE(ui16intExists));
    s.push_back(CUTE(ui16NotEqualAutomaticInCpp20));
    s.push_back(CUTE(ui16canbeadded));
    s.push_back(CUTE(ui16canbeaddedto));
    s.push_back(CUTE(ui16canbesubtracted));
    s.push_back(CUTE(ui16canbesubtractedfrom));
    s.push_back(CUTE(ui16canbemultiplied));
    s.push_back(CUTE(ui16canbemultipliedwith));
    s.push_back(CUTE(ui16canbedivided));
    s.push_back(CUTE(ui16canbedividedby));
    s.push_back(CUTE(ui16canbemoduloed));
    s.push_back(CUTE(ui16canbemoduloedby));
    s.push_back(CUTE(ui16canbeandedwith));
    s.push_back(CUTE(ui16canbeanded));
    s.push_back(CUTE(ui16canbeored));
    s.push_back(CUTE(ui16canbeoredwith));
    s.push_back(CUTE(ui16canbexoredwith));
    s.push_back(CUTE(ui16canbexored));
    s.push_back(CUTE(ui16canbenegated));
    s.push_back(CUTE(ui16canbeleftshifted));
    s.push_back(CUTE(ui16canbeleftshiftedby));
    s.push_back(CUTE(ui16canberightshifted));
    s.push_back(CUTE(ui16canberightshiftedby));
    s.push_back(CUTE(si8canbeaddednormal));
    s.push_back(CUTE(si8Negation));
    s.push_back(CUTE(si8negationminintthrows));
    s.push_back(CUTE(si8overflowIsDetected));
    s.push_back(CUTE(si8subtraction));
    s.push_back(CUTE(si8subtractionoverflowdetected));
    s.push_back(CUTE(si8multiplication));
    s.push_back(CUTE(si8division));
    s.push_back(CUTE(ui16canbepreincremented));
    s.push_back(CUTE(ui16canbepostincremented));
    s.push_back(CUTE(ui16canbepredecremented));
    s.push_back(CUTE(ui16canbepostdecremented));
	s.push_back(CUTE(checkedFromInt));
	s.push_back(CUTE(si8OutputAsInteger));
	s.push_back(CUTE(ui8OutputAsInteger));
	s.push_back(CUTE(ui16canbecompared));
	s.push_back(CUTE(ui16canNotbecomparedwithui8));
	s.push_back(CUTE(ui32CanNotbeComparedwithlong));
	s.push_back(CUTE(_testing::signedIntegerBoundaryTestResultRecovery));
    s.push_back(CUTE(si8preincrement));
    s.push_back(CUTE(si8postincrement));
    s.push_back(CUTE(si8predecrement));
    s.push_back(CUTE(si8postdecrement));
    s.push_back(CUTE(si16preincrement));
    s.push_back(CUTE(si16postincrement));
    s.push_back(CUTE(si16predecrement));
    s.push_back(CUTE(si16postdecrement));
    s.push_back(CUTE(si32preincrement));
    s.push_back(CUTE(si32postincrement));
    s.push_back(CUTE(si32predecrement));
    s.push_back(CUTE(si32postdecrement));
    s.push_back(CUTE(si64preincrement));
    s.push_back(CUTE(si64postincrement));
    s.push_back(CUTE(si64predecrement));
    s.push_back(CUTE(si64postdecrement));
    s.push_back(CUTE(ui8preincrement));
    s.push_back(CUTE(ui8postincrement));
    s.push_back(CUTE(ui8predecrement));
    s.push_back(CUTE(ui8postdecrement));
    s.push_back(CUTE(ui16preincrement));
    s.push_back(CUTE(ui16postincrement));
    s.push_back(CUTE(ui16predecrement));
    s.push_back(CUTE(ui16postdecrement));
    s.push_back(CUTE(ui32preincrement));
    s.push_back(CUTE(ui32postincrement));
    s.push_back(CUTE(ui32predecrement));
    s.push_back(CUTE(ui32postdecrement));
    s.push_back(CUTE(ui64preincrement));
    s.push_back(CUTE(ui64postincrement));
    s.push_back(CUTE(ui64predecrement));
    s.push_back(CUTE(ui64postdecrement));
	cute::xml_file_opener xmlfile(argc, argv);
    cute::xml_listener<cute::ide_listener<>> lis(xmlfile.out);
    auto runner = cute::makeRunner(lis, argc, argv);
    bool success = runner(s, "AllTests");
    success = runner(make_suite_CodeGenBenchmark(),"CodeGenBenchmark") && success;
    success &= runner(TestForZeroReturnAssertWithNDEBUG, "TestForZeroReturnAssertWithNDEBUG");
    cute::suite OverflowCheckedTests = make_suite_OverflowCheckedTests();
    success &= runner(OverflowCheckedTests, "OverflowCheckedTests");
    cute::suite NonBuiltInOverflowDetectionTests = make_suite_NonBuiltInOverflowDetectionTests();
    success &= runner(NonBuiltInOverflowDetectionTests, "NonBuiltInOverflowDetectionTests");
    return success;
}

int main(int argc, char const *argv[]) {
    return runAllTests(argc, argv) ? EXIT_SUCCESS : EXIT_FAILURE;
}
