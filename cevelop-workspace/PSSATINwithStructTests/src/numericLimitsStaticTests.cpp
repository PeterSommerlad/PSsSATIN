#include "satins.h"

// this file content is generated
// it tests all numeric limits members of all integer replacement types against their corresponding underlying types

/* File: ../generatenumericlmitstests.sh
#!/bin/sh

for safeint in ssi8 ssi16 ssi32 ssi64 sui8 sui16 sui32 sui64
do

for func in min max lowest epsilon round_error infinity quiet_NaN signaling_NaN denorm_min
do
  echo "static_assert(std::numeric_limits<$safeint>::$func() == from_int(std::numeric_limits<ULT<$safeint>>::$func()) );"


done

for variable in is_specialized digits digits10 max_digits10 is_signed is_integer is_exact radix min_exponent min_exponent10 max_exponent max_exponent10 \
                has_infinity has_quiet_NaN has_signaling_NaN has_denorm has_denorm_loss is_iec559 is_bounded tinyness_before round_style
do
  echo "static_assert(std::numeric_limits<$safeint>::$variable == std::numeric_limits<ULT<$safeint>>::$variable);"

done

  echo "static_assert(not std::numeric_limits<$safeint>::traps);"
  echo "static_assert(not std::numeric_limits<$safeint>::is_modulo);"

done


*/
using namespace satins;

static_assert(std::numeric_limits<ssi8>::min() == from_int(std::numeric_limits<ULT<ssi8>>::min()) );
static_assert(std::numeric_limits<ssi8>::max() == from_int(std::numeric_limits<ULT<ssi8>>::max()) );
static_assert(std::numeric_limits<ssi8>::lowest() == from_int(std::numeric_limits<ULT<ssi8>>::lowest()) );
static_assert(std::numeric_limits<ssi8>::epsilon() == from_int(std::numeric_limits<ULT<ssi8>>::epsilon()) );
static_assert(std::numeric_limits<ssi8>::round_error() == from_int(std::numeric_limits<ULT<ssi8>>::round_error()) );
static_assert(std::numeric_limits<ssi8>::infinity() == from_int(std::numeric_limits<ULT<ssi8>>::infinity()) );
static_assert(std::numeric_limits<ssi8>::quiet_NaN() == from_int(std::numeric_limits<ULT<ssi8>>::quiet_NaN()) );
static_assert(std::numeric_limits<ssi8>::signaling_NaN() == from_int(std::numeric_limits<ULT<ssi8>>::signaling_NaN()) );
static_assert(std::numeric_limits<ssi8>::denorm_min() == from_int(std::numeric_limits<ULT<ssi8>>::denorm_min()) );
static_assert(std::numeric_limits<ssi8>::is_specialized == std::numeric_limits<ULT<ssi8>>::is_specialized);
static_assert(std::numeric_limits<ssi8>::digits == std::numeric_limits<ULT<ssi8>>::digits);
static_assert(std::numeric_limits<ssi8>::digits10 == std::numeric_limits<ULT<ssi8>>::digits10);
static_assert(std::numeric_limits<ssi8>::max_digits10 == std::numeric_limits<ULT<ssi8>>::max_digits10);
static_assert(std::numeric_limits<ssi8>::is_signed == std::numeric_limits<ULT<ssi8>>::is_signed);
static_assert(std::numeric_limits<ssi8>::is_integer == std::numeric_limits<ULT<ssi8>>::is_integer);
static_assert(std::numeric_limits<ssi8>::is_exact == std::numeric_limits<ULT<ssi8>>::is_exact);
static_assert(std::numeric_limits<ssi8>::radix == std::numeric_limits<ULT<ssi8>>::radix);
static_assert(std::numeric_limits<ssi8>::min_exponent == std::numeric_limits<ULT<ssi8>>::min_exponent);
static_assert(std::numeric_limits<ssi8>::min_exponent10 == std::numeric_limits<ULT<ssi8>>::min_exponent10);
static_assert(std::numeric_limits<ssi8>::max_exponent == std::numeric_limits<ULT<ssi8>>::max_exponent);
static_assert(std::numeric_limits<ssi8>::max_exponent10 == std::numeric_limits<ULT<ssi8>>::max_exponent10);
static_assert(std::numeric_limits<ssi8>::has_infinity == std::numeric_limits<ULT<ssi8>>::has_infinity);
static_assert(std::numeric_limits<ssi8>::has_quiet_NaN == std::numeric_limits<ULT<ssi8>>::has_quiet_NaN);
static_assert(std::numeric_limits<ssi8>::has_signaling_NaN == std::numeric_limits<ULT<ssi8>>::has_signaling_NaN);
static_assert(std::numeric_limits<ssi8>::has_denorm == std::numeric_limits<ULT<ssi8>>::has_denorm);
static_assert(std::numeric_limits<ssi8>::has_denorm_loss == std::numeric_limits<ULT<ssi8>>::has_denorm_loss);
static_assert(std::numeric_limits<ssi8>::is_iec559 == std::numeric_limits<ULT<ssi8>>::is_iec559);
static_assert(std::numeric_limits<ssi8>::is_bounded == std::numeric_limits<ULT<ssi8>>::is_bounded);
static_assert(std::numeric_limits<ssi8>::tinyness_before == std::numeric_limits<ULT<ssi8>>::tinyness_before);
static_assert(std::numeric_limits<ssi8>::round_style == std::numeric_limits<ULT<ssi8>>::round_style);
static_assert(not std::numeric_limits<ssi8>::traps);
static_assert(not std::numeric_limits<ssi8>::is_modulo);
static_assert(std::numeric_limits<ssi16>::min() == from_int(std::numeric_limits<ULT<ssi16>>::min()) );
static_assert(std::numeric_limits<ssi16>::max() == from_int(std::numeric_limits<ULT<ssi16>>::max()) );
static_assert(std::numeric_limits<ssi16>::lowest() == from_int(std::numeric_limits<ULT<ssi16>>::lowest()) );
static_assert(std::numeric_limits<ssi16>::epsilon() == from_int(std::numeric_limits<ULT<ssi16>>::epsilon()) );
static_assert(std::numeric_limits<ssi16>::round_error() == from_int(std::numeric_limits<ULT<ssi16>>::round_error()) );
static_assert(std::numeric_limits<ssi16>::infinity() == from_int(std::numeric_limits<ULT<ssi16>>::infinity()) );
static_assert(std::numeric_limits<ssi16>::quiet_NaN() == from_int(std::numeric_limits<ULT<ssi16>>::quiet_NaN()) );
static_assert(std::numeric_limits<ssi16>::signaling_NaN() == from_int(std::numeric_limits<ULT<ssi16>>::signaling_NaN()) );
static_assert(std::numeric_limits<ssi16>::denorm_min() == from_int(std::numeric_limits<ULT<ssi16>>::denorm_min()) );
static_assert(std::numeric_limits<ssi16>::is_specialized == std::numeric_limits<ULT<ssi16>>::is_specialized);
static_assert(std::numeric_limits<ssi16>::digits == std::numeric_limits<ULT<ssi16>>::digits);
static_assert(std::numeric_limits<ssi16>::digits10 == std::numeric_limits<ULT<ssi16>>::digits10);
static_assert(std::numeric_limits<ssi16>::max_digits10 == std::numeric_limits<ULT<ssi16>>::max_digits10);
static_assert(std::numeric_limits<ssi16>::is_signed == std::numeric_limits<ULT<ssi16>>::is_signed);
static_assert(std::numeric_limits<ssi16>::is_integer == std::numeric_limits<ULT<ssi16>>::is_integer);
static_assert(std::numeric_limits<ssi16>::is_exact == std::numeric_limits<ULT<ssi16>>::is_exact);
static_assert(std::numeric_limits<ssi16>::radix == std::numeric_limits<ULT<ssi16>>::radix);
static_assert(std::numeric_limits<ssi16>::min_exponent == std::numeric_limits<ULT<ssi16>>::min_exponent);
static_assert(std::numeric_limits<ssi16>::min_exponent10 == std::numeric_limits<ULT<ssi16>>::min_exponent10);
static_assert(std::numeric_limits<ssi16>::max_exponent == std::numeric_limits<ULT<ssi16>>::max_exponent);
static_assert(std::numeric_limits<ssi16>::max_exponent10 == std::numeric_limits<ULT<ssi16>>::max_exponent10);
static_assert(std::numeric_limits<ssi16>::has_infinity == std::numeric_limits<ULT<ssi16>>::has_infinity);
static_assert(std::numeric_limits<ssi16>::has_quiet_NaN == std::numeric_limits<ULT<ssi16>>::has_quiet_NaN);
static_assert(std::numeric_limits<ssi16>::has_signaling_NaN == std::numeric_limits<ULT<ssi16>>::has_signaling_NaN);
static_assert(std::numeric_limits<ssi16>::has_denorm == std::numeric_limits<ULT<ssi16>>::has_denorm);
static_assert(std::numeric_limits<ssi16>::has_denorm_loss == std::numeric_limits<ULT<ssi16>>::has_denorm_loss);
static_assert(std::numeric_limits<ssi16>::is_iec559 == std::numeric_limits<ULT<ssi16>>::is_iec559);
static_assert(std::numeric_limits<ssi16>::is_bounded == std::numeric_limits<ULT<ssi16>>::is_bounded);
static_assert(std::numeric_limits<ssi16>::tinyness_before == std::numeric_limits<ULT<ssi16>>::tinyness_before);
static_assert(std::numeric_limits<ssi16>::round_style == std::numeric_limits<ULT<ssi16>>::round_style);
static_assert(not std::numeric_limits<ssi16>::traps);
static_assert(not std::numeric_limits<ssi16>::is_modulo);
static_assert(std::numeric_limits<ssi32>::min() == from_int(std::numeric_limits<ULT<ssi32>>::min()) );
static_assert(std::numeric_limits<ssi32>::max() == from_int(std::numeric_limits<ULT<ssi32>>::max()) );
static_assert(std::numeric_limits<ssi32>::lowest() == from_int(std::numeric_limits<ULT<ssi32>>::lowest()) );
static_assert(std::numeric_limits<ssi32>::epsilon() == from_int(std::numeric_limits<ULT<ssi32>>::epsilon()) );
static_assert(std::numeric_limits<ssi32>::round_error() == from_int(std::numeric_limits<ULT<ssi32>>::round_error()) );
static_assert(std::numeric_limits<ssi32>::infinity() == from_int(std::numeric_limits<ULT<ssi32>>::infinity()) );
static_assert(std::numeric_limits<ssi32>::quiet_NaN() == from_int(std::numeric_limits<ULT<ssi32>>::quiet_NaN()) );
static_assert(std::numeric_limits<ssi32>::signaling_NaN() == from_int(std::numeric_limits<ULT<ssi32>>::signaling_NaN()) );
static_assert(std::numeric_limits<ssi32>::denorm_min() == from_int(std::numeric_limits<ULT<ssi32>>::denorm_min()) );
static_assert(std::numeric_limits<ssi32>::is_specialized == std::numeric_limits<ULT<ssi32>>::is_specialized);
static_assert(std::numeric_limits<ssi32>::digits == std::numeric_limits<ULT<ssi32>>::digits);
static_assert(std::numeric_limits<ssi32>::digits10 == std::numeric_limits<ULT<ssi32>>::digits10);
static_assert(std::numeric_limits<ssi32>::max_digits10 == std::numeric_limits<ULT<ssi32>>::max_digits10);
static_assert(std::numeric_limits<ssi32>::is_signed == std::numeric_limits<ULT<ssi32>>::is_signed);
static_assert(std::numeric_limits<ssi32>::is_integer == std::numeric_limits<ULT<ssi32>>::is_integer);
static_assert(std::numeric_limits<ssi32>::is_exact == std::numeric_limits<ULT<ssi32>>::is_exact);
static_assert(std::numeric_limits<ssi32>::radix == std::numeric_limits<ULT<ssi32>>::radix);
static_assert(std::numeric_limits<ssi32>::min_exponent == std::numeric_limits<ULT<ssi32>>::min_exponent);
static_assert(std::numeric_limits<ssi32>::min_exponent10 == std::numeric_limits<ULT<ssi32>>::min_exponent10);
static_assert(std::numeric_limits<ssi32>::max_exponent == std::numeric_limits<ULT<ssi32>>::max_exponent);
static_assert(std::numeric_limits<ssi32>::max_exponent10 == std::numeric_limits<ULT<ssi32>>::max_exponent10);
static_assert(std::numeric_limits<ssi32>::has_infinity == std::numeric_limits<ULT<ssi32>>::has_infinity);
static_assert(std::numeric_limits<ssi32>::has_quiet_NaN == std::numeric_limits<ULT<ssi32>>::has_quiet_NaN);
static_assert(std::numeric_limits<ssi32>::has_signaling_NaN == std::numeric_limits<ULT<ssi32>>::has_signaling_NaN);
static_assert(std::numeric_limits<ssi32>::has_denorm == std::numeric_limits<ULT<ssi32>>::has_denorm);
static_assert(std::numeric_limits<ssi32>::has_denorm_loss == std::numeric_limits<ULT<ssi32>>::has_denorm_loss);
static_assert(std::numeric_limits<ssi32>::is_iec559 == std::numeric_limits<ULT<ssi32>>::is_iec559);
static_assert(std::numeric_limits<ssi32>::is_bounded == std::numeric_limits<ULT<ssi32>>::is_bounded);
static_assert(std::numeric_limits<ssi32>::tinyness_before == std::numeric_limits<ULT<ssi32>>::tinyness_before);
static_assert(std::numeric_limits<ssi32>::round_style == std::numeric_limits<ULT<ssi32>>::round_style);
static_assert(not std::numeric_limits<ssi32>::traps);
static_assert(not std::numeric_limits<ssi32>::is_modulo);
static_assert(std::numeric_limits<ssi64>::min() == from_int(std::numeric_limits<ULT<ssi64>>::min()) );
static_assert(std::numeric_limits<ssi64>::max() == from_int(std::numeric_limits<ULT<ssi64>>::max()) );
static_assert(std::numeric_limits<ssi64>::lowest() == from_int(std::numeric_limits<ULT<ssi64>>::lowest()) );
static_assert(std::numeric_limits<ssi64>::epsilon() == from_int(std::numeric_limits<ULT<ssi64>>::epsilon()) );
static_assert(std::numeric_limits<ssi64>::round_error() == from_int(std::numeric_limits<ULT<ssi64>>::round_error()) );
static_assert(std::numeric_limits<ssi64>::infinity() == from_int(std::numeric_limits<ULT<ssi64>>::infinity()) );
static_assert(std::numeric_limits<ssi64>::quiet_NaN() == from_int(std::numeric_limits<ULT<ssi64>>::quiet_NaN()) );
static_assert(std::numeric_limits<ssi64>::signaling_NaN() == from_int(std::numeric_limits<ULT<ssi64>>::signaling_NaN()) );
static_assert(std::numeric_limits<ssi64>::denorm_min() == from_int(std::numeric_limits<ULT<ssi64>>::denorm_min()) );
static_assert(std::numeric_limits<ssi64>::is_specialized == std::numeric_limits<ULT<ssi64>>::is_specialized);
static_assert(std::numeric_limits<ssi64>::digits == std::numeric_limits<ULT<ssi64>>::digits);
static_assert(std::numeric_limits<ssi64>::digits10 == std::numeric_limits<ULT<ssi64>>::digits10);
static_assert(std::numeric_limits<ssi64>::max_digits10 == std::numeric_limits<ULT<ssi64>>::max_digits10);
static_assert(std::numeric_limits<ssi64>::is_signed == std::numeric_limits<ULT<ssi64>>::is_signed);
static_assert(std::numeric_limits<ssi64>::is_integer == std::numeric_limits<ULT<ssi64>>::is_integer);
static_assert(std::numeric_limits<ssi64>::is_exact == std::numeric_limits<ULT<ssi64>>::is_exact);
static_assert(std::numeric_limits<ssi64>::radix == std::numeric_limits<ULT<ssi64>>::radix);
static_assert(std::numeric_limits<ssi64>::min_exponent == std::numeric_limits<ULT<ssi64>>::min_exponent);
static_assert(std::numeric_limits<ssi64>::min_exponent10 == std::numeric_limits<ULT<ssi64>>::min_exponent10);
static_assert(std::numeric_limits<ssi64>::max_exponent == std::numeric_limits<ULT<ssi64>>::max_exponent);
static_assert(std::numeric_limits<ssi64>::max_exponent10 == std::numeric_limits<ULT<ssi64>>::max_exponent10);
static_assert(std::numeric_limits<ssi64>::has_infinity == std::numeric_limits<ULT<ssi64>>::has_infinity);
static_assert(std::numeric_limits<ssi64>::has_quiet_NaN == std::numeric_limits<ULT<ssi64>>::has_quiet_NaN);
static_assert(std::numeric_limits<ssi64>::has_signaling_NaN == std::numeric_limits<ULT<ssi64>>::has_signaling_NaN);
static_assert(std::numeric_limits<ssi64>::has_denorm == std::numeric_limits<ULT<ssi64>>::has_denorm);
static_assert(std::numeric_limits<ssi64>::has_denorm_loss == std::numeric_limits<ULT<ssi64>>::has_denorm_loss);
static_assert(std::numeric_limits<ssi64>::is_iec559 == std::numeric_limits<ULT<ssi64>>::is_iec559);
static_assert(std::numeric_limits<ssi64>::is_bounded == std::numeric_limits<ULT<ssi64>>::is_bounded);
static_assert(std::numeric_limits<ssi64>::tinyness_before == std::numeric_limits<ULT<ssi64>>::tinyness_before);
static_assert(std::numeric_limits<ssi64>::round_style == std::numeric_limits<ULT<ssi64>>::round_style);
static_assert(not std::numeric_limits<ssi64>::traps);
static_assert(not std::numeric_limits<ssi64>::is_modulo);
static_assert(std::numeric_limits<sui8>::min() == from_int(std::numeric_limits<ULT<sui8>>::min()) );
static_assert(std::numeric_limits<sui8>::max() == from_int(std::numeric_limits<ULT<sui8>>::max()) );
static_assert(std::numeric_limits<sui8>::lowest() == from_int(std::numeric_limits<ULT<sui8>>::lowest()) );
static_assert(std::numeric_limits<sui8>::epsilon() == from_int(std::numeric_limits<ULT<sui8>>::epsilon()) );
static_assert(std::numeric_limits<sui8>::round_error() == from_int(std::numeric_limits<ULT<sui8>>::round_error()) );
static_assert(std::numeric_limits<sui8>::infinity() == from_int(std::numeric_limits<ULT<sui8>>::infinity()) );
static_assert(std::numeric_limits<sui8>::quiet_NaN() == from_int(std::numeric_limits<ULT<sui8>>::quiet_NaN()) );
static_assert(std::numeric_limits<sui8>::signaling_NaN() == from_int(std::numeric_limits<ULT<sui8>>::signaling_NaN()) );
static_assert(std::numeric_limits<sui8>::denorm_min() == from_int(std::numeric_limits<ULT<sui8>>::denorm_min()) );
static_assert(std::numeric_limits<sui8>::is_specialized == std::numeric_limits<ULT<sui8>>::is_specialized);
static_assert(std::numeric_limits<sui8>::digits == std::numeric_limits<ULT<sui8>>::digits);
static_assert(std::numeric_limits<sui8>::digits10 == std::numeric_limits<ULT<sui8>>::digits10);
static_assert(std::numeric_limits<sui8>::max_digits10 == std::numeric_limits<ULT<sui8>>::max_digits10);
static_assert(std::numeric_limits<sui8>::is_signed == std::numeric_limits<ULT<sui8>>::is_signed);
static_assert(std::numeric_limits<sui8>::is_integer == std::numeric_limits<ULT<sui8>>::is_integer);
static_assert(std::numeric_limits<sui8>::is_exact == std::numeric_limits<ULT<sui8>>::is_exact);
static_assert(std::numeric_limits<sui8>::radix == std::numeric_limits<ULT<sui8>>::radix);
static_assert(std::numeric_limits<sui8>::min_exponent == std::numeric_limits<ULT<sui8>>::min_exponent);
static_assert(std::numeric_limits<sui8>::min_exponent10 == std::numeric_limits<ULT<sui8>>::min_exponent10);
static_assert(std::numeric_limits<sui8>::max_exponent == std::numeric_limits<ULT<sui8>>::max_exponent);
static_assert(std::numeric_limits<sui8>::max_exponent10 == std::numeric_limits<ULT<sui8>>::max_exponent10);
static_assert(std::numeric_limits<sui8>::has_infinity == std::numeric_limits<ULT<sui8>>::has_infinity);
static_assert(std::numeric_limits<sui8>::has_quiet_NaN == std::numeric_limits<ULT<sui8>>::has_quiet_NaN);
static_assert(std::numeric_limits<sui8>::has_signaling_NaN == std::numeric_limits<ULT<sui8>>::has_signaling_NaN);
static_assert(std::numeric_limits<sui8>::has_denorm == std::numeric_limits<ULT<sui8>>::has_denorm);
static_assert(std::numeric_limits<sui8>::has_denorm_loss == std::numeric_limits<ULT<sui8>>::has_denorm_loss);
static_assert(std::numeric_limits<sui8>::is_iec559 == std::numeric_limits<ULT<sui8>>::is_iec559);
static_assert(std::numeric_limits<sui8>::is_bounded == std::numeric_limits<ULT<sui8>>::is_bounded);
static_assert(std::numeric_limits<sui8>::tinyness_before == std::numeric_limits<ULT<sui8>>::tinyness_before);
static_assert(std::numeric_limits<sui8>::round_style == std::numeric_limits<ULT<sui8>>::round_style);
static_assert(not std::numeric_limits<sui8>::traps);
static_assert(not std::numeric_limits<sui8>::is_modulo);
static_assert(std::numeric_limits<sui16>::min() == from_int(std::numeric_limits<ULT<sui16>>::min()) );
static_assert(std::numeric_limits<sui16>::max() == from_int(std::numeric_limits<ULT<sui16>>::max()) );
static_assert(std::numeric_limits<sui16>::lowest() == from_int(std::numeric_limits<ULT<sui16>>::lowest()) );
static_assert(std::numeric_limits<sui16>::epsilon() == from_int(std::numeric_limits<ULT<sui16>>::epsilon()) );
static_assert(std::numeric_limits<sui16>::round_error() == from_int(std::numeric_limits<ULT<sui16>>::round_error()) );
static_assert(std::numeric_limits<sui16>::infinity() == from_int(std::numeric_limits<ULT<sui16>>::infinity()) );
static_assert(std::numeric_limits<sui16>::quiet_NaN() == from_int(std::numeric_limits<ULT<sui16>>::quiet_NaN()) );
static_assert(std::numeric_limits<sui16>::signaling_NaN() == from_int(std::numeric_limits<ULT<sui16>>::signaling_NaN()) );
static_assert(std::numeric_limits<sui16>::denorm_min() == from_int(std::numeric_limits<ULT<sui16>>::denorm_min()) );
static_assert(std::numeric_limits<sui16>::is_specialized == std::numeric_limits<ULT<sui16>>::is_specialized);
static_assert(std::numeric_limits<sui16>::digits == std::numeric_limits<ULT<sui16>>::digits);
static_assert(std::numeric_limits<sui16>::digits10 == std::numeric_limits<ULT<sui16>>::digits10);
static_assert(std::numeric_limits<sui16>::max_digits10 == std::numeric_limits<ULT<sui16>>::max_digits10);
static_assert(std::numeric_limits<sui16>::is_signed == std::numeric_limits<ULT<sui16>>::is_signed);
static_assert(std::numeric_limits<sui16>::is_integer == std::numeric_limits<ULT<sui16>>::is_integer);
static_assert(std::numeric_limits<sui16>::is_exact == std::numeric_limits<ULT<sui16>>::is_exact);
static_assert(std::numeric_limits<sui16>::radix == std::numeric_limits<ULT<sui16>>::radix);
static_assert(std::numeric_limits<sui16>::min_exponent == std::numeric_limits<ULT<sui16>>::min_exponent);
static_assert(std::numeric_limits<sui16>::min_exponent10 == std::numeric_limits<ULT<sui16>>::min_exponent10);
static_assert(std::numeric_limits<sui16>::max_exponent == std::numeric_limits<ULT<sui16>>::max_exponent);
static_assert(std::numeric_limits<sui16>::max_exponent10 == std::numeric_limits<ULT<sui16>>::max_exponent10);
static_assert(std::numeric_limits<sui16>::has_infinity == std::numeric_limits<ULT<sui16>>::has_infinity);
static_assert(std::numeric_limits<sui16>::has_quiet_NaN == std::numeric_limits<ULT<sui16>>::has_quiet_NaN);
static_assert(std::numeric_limits<sui16>::has_signaling_NaN == std::numeric_limits<ULT<sui16>>::has_signaling_NaN);
static_assert(std::numeric_limits<sui16>::has_denorm == std::numeric_limits<ULT<sui16>>::has_denorm);
static_assert(std::numeric_limits<sui16>::has_denorm_loss == std::numeric_limits<ULT<sui16>>::has_denorm_loss);
static_assert(std::numeric_limits<sui16>::is_iec559 == std::numeric_limits<ULT<sui16>>::is_iec559);
static_assert(std::numeric_limits<sui16>::is_bounded == std::numeric_limits<ULT<sui16>>::is_bounded);
static_assert(std::numeric_limits<sui16>::tinyness_before == std::numeric_limits<ULT<sui16>>::tinyness_before);
static_assert(std::numeric_limits<sui16>::round_style == std::numeric_limits<ULT<sui16>>::round_style);
static_assert(not std::numeric_limits<sui16>::traps);
static_assert(not std::numeric_limits<sui16>::is_modulo);
static_assert(std::numeric_limits<sui32>::min() == from_int(std::numeric_limits<ULT<sui32>>::min()) );
static_assert(std::numeric_limits<sui32>::max() == from_int(std::numeric_limits<ULT<sui32>>::max()) );
static_assert(std::numeric_limits<sui32>::lowest() == from_int(std::numeric_limits<ULT<sui32>>::lowest()) );
static_assert(std::numeric_limits<sui32>::epsilon() == from_int(std::numeric_limits<ULT<sui32>>::epsilon()) );
static_assert(std::numeric_limits<sui32>::round_error() == from_int(std::numeric_limits<ULT<sui32>>::round_error()) );
static_assert(std::numeric_limits<sui32>::infinity() == from_int(std::numeric_limits<ULT<sui32>>::infinity()) );
static_assert(std::numeric_limits<sui32>::quiet_NaN() == from_int(std::numeric_limits<ULT<sui32>>::quiet_NaN()) );
static_assert(std::numeric_limits<sui32>::signaling_NaN() == from_int(std::numeric_limits<ULT<sui32>>::signaling_NaN()) );
static_assert(std::numeric_limits<sui32>::denorm_min() == from_int(std::numeric_limits<ULT<sui32>>::denorm_min()) );
static_assert(std::numeric_limits<sui32>::is_specialized == std::numeric_limits<ULT<sui32>>::is_specialized);
static_assert(std::numeric_limits<sui32>::digits == std::numeric_limits<ULT<sui32>>::digits);
static_assert(std::numeric_limits<sui32>::digits10 == std::numeric_limits<ULT<sui32>>::digits10);
static_assert(std::numeric_limits<sui32>::max_digits10 == std::numeric_limits<ULT<sui32>>::max_digits10);
static_assert(std::numeric_limits<sui32>::is_signed == std::numeric_limits<ULT<sui32>>::is_signed);
static_assert(std::numeric_limits<sui32>::is_integer == std::numeric_limits<ULT<sui32>>::is_integer);
static_assert(std::numeric_limits<sui32>::is_exact == std::numeric_limits<ULT<sui32>>::is_exact);
static_assert(std::numeric_limits<sui32>::radix == std::numeric_limits<ULT<sui32>>::radix);
static_assert(std::numeric_limits<sui32>::min_exponent == std::numeric_limits<ULT<sui32>>::min_exponent);
static_assert(std::numeric_limits<sui32>::min_exponent10 == std::numeric_limits<ULT<sui32>>::min_exponent10);
static_assert(std::numeric_limits<sui32>::max_exponent == std::numeric_limits<ULT<sui32>>::max_exponent);
static_assert(std::numeric_limits<sui32>::max_exponent10 == std::numeric_limits<ULT<sui32>>::max_exponent10);
static_assert(std::numeric_limits<sui32>::has_infinity == std::numeric_limits<ULT<sui32>>::has_infinity);
static_assert(std::numeric_limits<sui32>::has_quiet_NaN == std::numeric_limits<ULT<sui32>>::has_quiet_NaN);
static_assert(std::numeric_limits<sui32>::has_signaling_NaN == std::numeric_limits<ULT<sui32>>::has_signaling_NaN);
static_assert(std::numeric_limits<sui32>::has_denorm == std::numeric_limits<ULT<sui32>>::has_denorm);
static_assert(std::numeric_limits<sui32>::has_denorm_loss == std::numeric_limits<ULT<sui32>>::has_denorm_loss);
static_assert(std::numeric_limits<sui32>::is_iec559 == std::numeric_limits<ULT<sui32>>::is_iec559);
static_assert(std::numeric_limits<sui32>::is_bounded == std::numeric_limits<ULT<sui32>>::is_bounded);
static_assert(std::numeric_limits<sui32>::tinyness_before == std::numeric_limits<ULT<sui32>>::tinyness_before);
static_assert(std::numeric_limits<sui32>::round_style == std::numeric_limits<ULT<sui32>>::round_style);
static_assert(not std::numeric_limits<sui32>::traps);
static_assert(not std::numeric_limits<sui32>::is_modulo);
static_assert(std::numeric_limits<sui64>::min() == from_int(std::numeric_limits<ULT<sui64>>::min()) );
static_assert(std::numeric_limits<sui64>::max() == from_int(std::numeric_limits<ULT<sui64>>::max()) );
static_assert(std::numeric_limits<sui64>::lowest() == from_int(std::numeric_limits<ULT<sui64>>::lowest()) );
static_assert(std::numeric_limits<sui64>::epsilon() == from_int(std::numeric_limits<ULT<sui64>>::epsilon()) );
static_assert(std::numeric_limits<sui64>::round_error() == from_int(std::numeric_limits<ULT<sui64>>::round_error()) );
static_assert(std::numeric_limits<sui64>::infinity() == from_int(std::numeric_limits<ULT<sui64>>::infinity()) );
static_assert(std::numeric_limits<sui64>::quiet_NaN() == from_int(std::numeric_limits<ULT<sui64>>::quiet_NaN()) );
static_assert(std::numeric_limits<sui64>::signaling_NaN() == from_int(std::numeric_limits<ULT<sui64>>::signaling_NaN()) );
static_assert(std::numeric_limits<sui64>::denorm_min() == from_int(std::numeric_limits<ULT<sui64>>::denorm_min()) );
static_assert(std::numeric_limits<sui64>::is_specialized == std::numeric_limits<ULT<sui64>>::is_specialized);
static_assert(std::numeric_limits<sui64>::digits == std::numeric_limits<ULT<sui64>>::digits);
static_assert(std::numeric_limits<sui64>::digits10 == std::numeric_limits<ULT<sui64>>::digits10);
static_assert(std::numeric_limits<sui64>::max_digits10 == std::numeric_limits<ULT<sui64>>::max_digits10);
static_assert(std::numeric_limits<sui64>::is_signed == std::numeric_limits<ULT<sui64>>::is_signed);
static_assert(std::numeric_limits<sui64>::is_integer == std::numeric_limits<ULT<sui64>>::is_integer);
static_assert(std::numeric_limits<sui64>::is_exact == std::numeric_limits<ULT<sui64>>::is_exact);
static_assert(std::numeric_limits<sui64>::radix == std::numeric_limits<ULT<sui64>>::radix);
static_assert(std::numeric_limits<sui64>::min_exponent == std::numeric_limits<ULT<sui64>>::min_exponent);
static_assert(std::numeric_limits<sui64>::min_exponent10 == std::numeric_limits<ULT<sui64>>::min_exponent10);
static_assert(std::numeric_limits<sui64>::max_exponent == std::numeric_limits<ULT<sui64>>::max_exponent);
static_assert(std::numeric_limits<sui64>::max_exponent10 == std::numeric_limits<ULT<sui64>>::max_exponent10);
static_assert(std::numeric_limits<sui64>::has_infinity == std::numeric_limits<ULT<sui64>>::has_infinity);
static_assert(std::numeric_limits<sui64>::has_quiet_NaN == std::numeric_limits<ULT<sui64>>::has_quiet_NaN);
static_assert(std::numeric_limits<sui64>::has_signaling_NaN == std::numeric_limits<ULT<sui64>>::has_signaling_NaN);
static_assert(std::numeric_limits<sui64>::has_denorm == std::numeric_limits<ULT<sui64>>::has_denorm);
static_assert(std::numeric_limits<sui64>::has_denorm_loss == std::numeric_limits<ULT<sui64>>::has_denorm_loss);
static_assert(std::numeric_limits<sui64>::is_iec559 == std::numeric_limits<ULT<sui64>>::is_iec559);
static_assert(std::numeric_limits<sui64>::is_bounded == std::numeric_limits<ULT<sui64>>::is_bounded);
static_assert(std::numeric_limits<sui64>::tinyness_before == std::numeric_limits<ULT<sui64>>::tinyness_before);
static_assert(std::numeric_limits<sui64>::round_style == std::numeric_limits<ULT<sui64>>::round_style);
static_assert(not std::numeric_limits<sui64>::traps);
static_assert(not std::numeric_limits<sui64>::is_modulo);
