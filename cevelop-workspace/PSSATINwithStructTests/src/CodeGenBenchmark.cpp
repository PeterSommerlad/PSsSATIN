#include "CodeGenBenchmark.h"
#include "cute.h"
#include <initializer_list>

#include <vector>
#include <numeric>

#include "satins.h"

using namespace satins;
using namespace satins::detail_;


template<typename INT>
struct operations {
    operations(std::initializer_list<INT> seedvalues):values{seedvalues}{};
    std::vector<INT> values;

    auto sum() const {
        auto const initialvalue = INT{0}; // +INT{}; // triggers promotion
        return std::accumulate(begin(values),end(values),initialvalue);
    }
    auto product() const {
        auto const initialvalue = INT{1}; // +INT{}; // triggers promotion
        return std::accumulate(begin(values),end(values),initialvalue,[](auto l, auto r){return l*r;});
    }
    auto sumthirds() const {
        auto const initialvalue = INT{0}; // +INT{}; // triggers promotion
        return std::accumulate(begin(values),end(values),initialvalue,[](auto l, auto r){return l+(r/INT{3});});
    }
    auto sumnegative() const {
        auto const initialvalue = INT{}; // +INT{}; // triggers promotion
        return std::accumulate(begin(values),end(values),initialvalue,[](auto l, auto r){return l-r;});
    }
};

std::initializer_list<int8_t> i8_seed{1,1,2,3,5,8/*,13,21,34,55,89*/};
std::initializer_list<satins::ssi8> si8_seed{1_ssi8,1_ssi8,2_ssi8,3_ssi8,5_ssi8,8_ssi8/*,13_ssi8,21_ssi8,34_ssi8,55_ssi8,89_ssi8*/};

auto sum(operations<int8_t> const &ops){
    return ops.sum();
}
auto product(operations<int8_t> const &ops){
    return ops.product();
}
auto sumthirds(operations<int8_t> const &&ops){
    return ops.sumthirds();
}
auto sumnegative(operations<int8_t> const &ops){
    return ops.sumnegative();
}

auto sum(operations<ssi8> const &ops){
    return ops.sum();
}
auto product(operations<ssi8> const &ops){
    return ops.product();
}
auto sumthirds(operations<ssi8> const &&ops){
    return ops.sumthirds();
}
auto sumnegative(operations<ssi8> const &ops){
    return ops.sumnegative();
}




void codegenAdditionTest() {
    auto resi8 = sum(operations<int8_t>{i8_seed});
    auto ressi8 =sum( operations<ssi8>{si8_seed});
    ASSERT_EQUAL(resi8,promote_keep_signedness(ressi8));
}

void codegenMultiplicationTest(){
    auto resi8 = product(operations<int8_t>{i8_seed});
//    auto ressi8 = product( operations<si8>{si8_seed});
//    ASSERT_EQUAL(resi8,promote_keep_signedness(ressi8));
//    ASSERT_EQUAL(from_int(resi8),ressi8);
    ASSERTM("expect signed integer overflow not detected by ubsan",resi8 < 0);
//    ASSERTM("expect signed integer overflow not detected by ubsan",ressi8 < 0_ssi8);
}
void codegenSumThirdsTest(){
    auto resi8 = sumthirds(operations<int8_t>{i8_seed});
    auto ressi8 = sumthirds( operations<ssi8>{si8_seed});
    ASSERT_EQUAL(resi8,promote_keep_signedness(ressi8));
}
void codegenSubtractionTest(){
    auto resi8 = sumnegative(operations<int8_t>{i8_seed});
    auto ressi8 = sumnegative( operations<ssi8>{si8_seed});
    ASSERT_EQUAL(resi8,promote_keep_signedness(ressi8));
}

namespace int16 {
std::initializer_list<int16_t> i16_seed{1,1,2,3,5,8,13,21,34,55};
std::initializer_list<satins::ssi16> si16_seed{1_ssi16,1_ssi16,2_ssi16,3_ssi16,5_ssi16,8_ssi16,13_ssi16,21_ssi16,34_ssi16,55_ssi16};

auto sum(operations<int16_t> const &ops){
    return ops.sum();
}
auto product(operations<int16_t> const &ops){
    return ops.product();
}
auto sumthirds(operations<int16_t> const &&ops){
    return ops.sumthirds();
}
auto sumnegative(operations<int16_t> const &ops){
    return ops.sumnegative();
}

auto sum(operations<ssi16> const &ops){
    return ops.sum();
}
auto product(operations<ssi16> const &ops){
    return ops.product();
}
auto sumthirds(operations<ssi16> const &&ops){
    return ops.sumthirds();
}
auto sumnegative(operations<ssi16> const &ops){
    return ops.sumnegative();
}
void codegenAdditionTest() {
    auto resi16 = sum(operations<int16_t>{i16_seed});
    auto ressi16 =sum( operations<ssi16>{si16_seed});
    ASSERT_EQUAL(resi16,promote_keep_signedness(ressi16));
}

//#pragma GCC diagnostic push
//#if defined(__GNUG__)
//#if defined(__clang__)
//#pragma clang diagnostic ignored "-Wunused-variable"
//
//#else
//#pragma GCC diagnostic ignored "-Wunused-variable"
//#endif
//#endif


void codegenMultiplicationTest(){
    auto resi16 = product(operations<int16_t>{i16_seed});
    auto ressi16 = product( operations<ssi16>{si16_seed});
    ASSERT_EQUAL(std::numeric_limits<ssi16>::max(),ressi16);
//    ASSERT_THROWS(   [[maybe_unused]] auto ressi16 = product( operations<ssi16>{si16_seed}); , char const *);
//    ASSERT_EQUAL(resi16,promote_keep_signedness(ressi16));
//    ASSERT_EQUAL(from_int(resi16),ressi16);
    ASSERTM("expect signed integer overflow not detected by ubsan",resi16 < 0);
//    ASSERTM("expect signed integer overflow not detected by ubsan",ressi16 < 0_ssi16);
    // no detection of overflow by UBSAN

}
void codegenSumThirdsTest(){
    auto resi16 = sumthirds(operations<int16_t>{i16_seed});
    auto ressi16 = sumthirds( operations<ssi16>{si16_seed});
    ASSERT_EQUAL(resi16,promote_keep_signedness(ressi16));
}
void codegenSubtractionTest(){
    auto resi16 = sumnegative(operations<int16_t>{i16_seed});
    auto ressi16 = sumnegative( operations<ssi16>{si16_seed});
    ASSERT_EQUAL(resi16,promote_keep_signedness(ressi16));
}

}
namespace int32 {
std::initializer_list<int32_t> i32_seed{1,1,2,3,5,8,13,21,34,55,89};
std::initializer_list<satins::ssi32> si32_seed{1_ssi32,1_ssi32,2_ssi32,3_ssi32,5_ssi32,8_ssi32,13_ssi32,21_ssi32,34_ssi32,55_ssi32,89_ssi32};

auto sum(operations<int32_t> const &ops){
    return ops.sum();
}
auto product(operations<int32_t> const &ops){
    return ops.product();
}
auto sumthirds(operations<int32_t> const &&ops){
    return ops.sumthirds();
}
auto sumnegative(operations<int32_t> const &ops){
    return ops.sumnegative();
}

auto sum(operations<ssi32> const &ops){
    return ops.sum();
}
auto product(operations<ssi32> const &ops){
    return ops.product();
}
auto sumthirds(operations<ssi32> const &&ops){
    return ops.sumthirds();
}
auto sumnegative(operations<ssi32> const &ops){
    return ops.sumnegative();
}
void codegenAdditionTest() {
    auto resi32 = sum(operations<int32_t>{i32_seed});
    auto ressi32 =sum( operations<ssi32>{si32_seed});
    ASSERT_EQUAL(resi32,promote_keep_signedness(ressi32));
}

void codegenMultiplicationTest(){
    auto const resi32 = product(operations<int32_t> { i32_seed });
    auto ressi32 = product( operations<ssi32>{si32_seed});
    ASSERT_EQUAL(std::numeric_limits<ssi32>::max(),ressi32);
//    ASSERT_THROWS([[maybe_unused]] auto ressi32 = product( operations<ssi32>{si32_seed}); , char const *);
//    ASSERT_EQUAL(resi32,promote_keep_signedness(ressi32));
//    ASSERT_EQUAL(from_int(resi32),ressi32);
    ASSERTM("expect signed integer overflow detected by ubsan",resi32 < 0);
//    ASSERTM("expect signed integer overflow not detected by ubsan",ressi32 < 0_ssi32);
    // ../src/CodeGenBenchmark.cpp:23:99: runtime error: signed integer overflow: 122522400 * 89 cannot be represented in type 'int'

}
void codegenSumThirdsTest(){
    auto resi32 = sumthirds(operations<int32_t>{i32_seed});
    auto ressi32 = sumthirds( operations<ssi32>{si32_seed});
    ASSERT_EQUAL(resi32,promote_keep_signedness(ressi32));
}
void codegenSubtractionTest(){
    auto resi32 = sumnegative(operations<int32_t>{i32_seed});
    auto ressi32 = sumnegative( operations<ssi32>{si32_seed});
    ASSERT_EQUAL(resi32,promote_keep_signedness(ressi32));
}

}
namespace int64 {
constexpr std::initializer_list<int64_t> i64_seed{1,1,2,3,5,8,13,21,34,55,89,
                                        144,233,377,610};
constexpr std::initializer_list<satins::ssi64> si64_seed{1_ssi64,1_ssi64,2_ssi64,3_ssi64,5_ssi64,8_ssi64,13_ssi64,21_ssi64,34_ssi64,55_ssi64,89_ssi64,
    144_ssi64,233_ssi64,377_ssi64,610_ssi64};

auto sum(operations<int64_t> const &ops){
    return ops.sum();
}
auto product(operations<int64_t> const &ops){
    return ops.product();
}
auto sumthirds(operations<int64_t> const &&ops){
    return ops.sumthirds();
}
auto sumnegative(operations<int64_t> const &ops){
    return ops.sumnegative();
}

auto sum(operations<ssi64> const &ops){
    return ops.sum();
}
auto product(operations<ssi64> const &ops){
    return ops.product();
}
auto sumthirds(operations<ssi64> const &&ops){
    return ops.sumthirds();
}
auto sumnegative(operations<ssi64> const &ops){
    return ops.sumnegative();
}
void codegenAdditionTest() {
    auto resi64 = sum(operations<int64_t>{i64_seed});
    auto ressi64 =sum( operations<ssi64>{si64_seed});
    ASSERT_EQUAL(resi64,promote_keep_signedness(ressi64));
}

void codegenMultiplicationTest(){
    auto resi64 = product(operations<int64_t>{i64_seed});
    auto ressi64 = product( operations<ssi64>{si64_seed});
    ASSERT_EQUAL(std::numeric_limits<ssi64>::max(),ressi64);
//    ASSERT_THROWS([[maybe_unused]] auto ressi64 = product( operations<ssi64>{si64_seed}); , char const *);
//    ASSERT_EQUAL(resi64,promote_keep_signedness(ressi64));
//    ASSERT_EQUAL(from_int(resi64),ressi64);
    ASSERTM("expect signed integer overflow detected by ubsan",resi64 < 0);
//    ASSERTM("expect signed integer overflow not detected by ubsan",ressi64 < 0_ssi64);
    // ../src/CodeGenBenchmark.cpp:23:99: runtime error: signed integer overflow: 137932073613734400 * 610 cannot be represented in type 'long long int'

}
void codegenSumThirdsTest(){
    auto resi64 = sumthirds(operations<int64_t>{i64_seed});
    auto ressi64 = sumthirds( operations<ssi64>{si64_seed});
    ASSERT_EQUAL(resi64,promote_keep_signedness(ressi64));
}
void codegenSubtractionTest(){
    auto resi64 = sumnegative(operations<int64_t>{i64_seed});
    auto ressi64 = sumnegative( operations<ssi64>{si64_seed});
    ASSERT_EQUAL(resi64,promote_keep_signedness(ressi64));
}

}
namespace uint32 {
std::initializer_list<uint32_t> ui32_seed{1,1,2,3,5,8,13,21,34,55,89};
std::initializer_list<satins::sui32> uui32_seed{1_sui32,1_sui32,2_sui32,3_sui32,5_sui32,8_sui32,13_sui32,21_sui32,34_sui32,55_sui32,89_sui32};

auto sum(operations<uint32_t> const &ops){
    return ops.sum();
}
auto product(operations<uint32_t> const &ops){
    return ops.product();
}
auto sumthirds(operations<uint32_t> const &&ops){
    return ops.sumthirds();
}
auto sumnegative(operations<uint32_t> const &ops){
    return ops.sumnegative();
}

auto sum(operations<sui32> const &ops){
    return ops.sum();
}
auto product(operations<sui32> const &ops){
    return ops.product();
}
auto sumthirds(operations<sui32> const &&ops){
    return ops.sumthirds();
}
auto sumnegative(operations<sui32> const &ops){
    return ops.sumnegative();
}
void codegenAdditionTest() {
    auto reui32 = sum(operations<uint32_t>{ui32_seed});
    auto resui32 =sum( operations<sui32>{uui32_seed});
    ASSERT_EQUAL(reui32,promote_keep_signedness(resui32));
}

void codegenMultiplicationTest(){
    [[maybe_unused]] auto reui32 = product(operations<uint32_t>{ui32_seed});
    auto resui32 = product( operations<sui32>{uui32_seed});
    ASSERT_EQUAL(std::numeric_limits<sui32>::max(),resui32);
    //ASSERT_THROWS([[maybe_unused]] auto resui32 = product( operations<sui32>{uui32_seed}); , char const *);
    //ASSERT_EQUAL(reui32,promote_keep_signedness(resui32));
}
void codegenSumThirdsTest(){
    auto reui32 = sumthirds(operations<uint32_t>{ui32_seed});
    auto resui32 = sumthirds( operations<sui32>{uui32_seed});
    ASSERT_EQUAL(reui32,promote_keep_signedness(resui32));
}
void codegenSubtractionTest(){
    [[maybe_unused]] auto reui32 = sumnegative(operations<uint32_t>{ui32_seed});
    [[maybe_unused]] auto resui32 = sumnegative( operations<sui32>{uui32_seed});
    ASSERT_EQUAL(0_sui32,resui32); // underflow
}

}


void testUBSanWithSignedOverflow(){
	unsigned short x=0xffffu;

	auto y = x * x; // UBSAN: ../src/CodeGenBenchmark.cpp:46:13: runtime error: signed integer overflow: 65535 * 65535 cannot be represented in type 'int'

	ASSERT_EQUAL(0xffffu*0xffffu,y);
}


void testUnSignedOverflowThrows(){
    auto x=0xffff_sui16;
    ASSERT_EQUAL(std::numeric_limits<satins::sui16>::max(), x * x);
//    ASSERT_THROWS(std::ignore = x * x, char const *);

}


cute::suite make_suite_CodeGenBenchmark() {
	cute::suite s { };
	s.push_back(CUTE(codegenAdditionTest));
	s.push_back(CUTE(testUBSanWithSignedOverflow));
	s.push_back(CUTE(testUnSignedOverflowThrows));
	s.push_back(CUTE(codegenMultiplicationTest));
	s.push_back(CUTE(codegenSumThirdsTest));
	s.push_back(CUTE(codegenSubtractionTest));
	s.push_back(CUTE(int32::codegenAdditionTest));
	s.push_back(CUTE(int32::codegenMultiplicationTest));
	s.push_back(CUTE(int32::codegenSumThirdsTest));
	s.push_back(CUTE(int32::codegenSubtractionTest));
	s.push_back(CUTE(uint32::codegenAdditionTest));
	s.push_back(CUTE(uint32::codegenMultiplicationTest));
	s.push_back(CUTE(uint32::codegenSumThirdsTest));
	s.push_back(CUTE(uint32::codegenSubtractionTest));
	s.push_back(CUTE(int16::codegenAdditionTest));
	s.push_back(CUTE(int16::codegenMultiplicationTest));
	s.push_back(CUTE(int16::codegenSumThirdsTest));
	s.push_back(CUTE(int16::codegenSubtractionTest));
	s.push_back(CUTE(int64::codegenAdditionTest));
	s.push_back(CUTE(int64::codegenMultiplicationTest));
	s.push_back(CUTE(int64::codegenSumThirdsTest));
	s.push_back(CUTE(int64::codegenSubtractionTest));
	return s;
}
