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
