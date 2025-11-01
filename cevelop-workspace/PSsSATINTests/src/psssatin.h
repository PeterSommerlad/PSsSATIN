#ifndef SRC_PSSSATIN_
#define SRC_PSSSATIN_

#include <cstdint>
#include <type_traits>
#include <iosfwd>
#include <limits>
#include <climits>
#if __cplusplus > 202302L
#include <numeric> // C++26 saturation arithmetic
#endif

namespace psssatin { // Peter Sommerlad's simple SATuration Integral Numbers PSsSATIN

// unsigned 
enum class [[nodiscard]] sui8: std::uint8_t { psssatin_tag_to_prevent_mixing_other_enums };
enum class [[nodiscard]]sui16: std::uint16_t{ psssatin_tag_to_prevent_mixing_other_enums };
enum class [[nodiscard]]sui32: std::uint32_t{ psssatin_tag_to_prevent_mixing_other_enums };
enum class [[nodiscard]]sui64: std::uint64_t{ psssatin_tag_to_prevent_mixing_other_enums };

inline namespace literals {
consteval
sui8 operator""_sui8(unsigned long long val) {
    if (val <= std::numeric_limits<std::underlying_type_t<sui8>>::max()) {
        return sui8(val);
    } else {
        throw "integral constant too large"; // trigger compile-time error
    }
}


consteval
sui16 operator""_sui16(unsigned long long val) {
    if (val <= std::numeric_limits<std::underlying_type_t<sui16>>::max()) {
        return sui16(val);
    } else {
        throw "integral constant too large"; // trigger compile-time error
    }
}


consteval
sui32 operator""_sui32(unsigned long long val) {
    if (val <= std::numeric_limits<std::underlying_type_t<sui32>>::max()) {
        return sui32(val);
    } else {
        throw "integral constant too large"; // trigger compile-time error
    }
}


consteval
sui64 operator""_sui64(unsigned long long val) {
    if constexpr (sizeof(sui64) < sizeof(val)){
        if (val > 0xffff'ffff'fffffffful) {
            throw "integral constant too large"; // trigger compile-time error
        }
    }
    return sui64(val);
}

}
// signed
enum class [[nodiscard]]ssi8 : std::int8_t { psssatin_tag_to_prevent_mixing_other_enums };
enum class [[nodiscard]]ssi16: std::int16_t{ psssatin_tag_to_prevent_mixing_other_enums };
enum class [[nodiscard]]ssi32: std::int32_t{ psssatin_tag_to_prevent_mixing_other_enums };
enum class [[nodiscard]]ssi64: std::int64_t{ psssatin_tag_to_prevent_mixing_other_enums };

inline namespace literals {
consteval
ssi8 operator""_ssi8(unsigned long long val) {
    if (val <= std::numeric_limits<std::underlying_type_t<ssi8>>::max()) {
        return ssi8(val);
    } else {
        throw "integral constant too large"; // trigger compile-time error
    }
}


consteval
ssi16 operator""_ssi16(unsigned long long val) {
    if (val <= std::numeric_limits<std::underlying_type_t<ssi16>>::max()) {
        return ssi16(val);
    } else {
        throw "integral constant too large"; // trigger compile-time error
    }
}


consteval
ssi32 operator""_ssi32(unsigned long long val) {
    if (val <= std::numeric_limits<std::underlying_type_t<ssi32>>::max()) {
        return ssi32(val);
    } else {
        throw "integral constant too large"; // trigger compile-time error
    }
}


consteval
ssi64 operator""_ssi64(unsigned long long val) {
    if (val <= std::numeric_limits<std::underlying_type_t<ssi64>>::max()) {
        return ssi64(val);
    } else {
        throw "integral constant too large"; // trigger compile-time error
    }
}
}




template<typename T>
using plain = std::remove_cvref_t<T>;

template<typename T>
concept an_enum = std::is_enum_v<plain<T>>;


namespace detail_ {
// from C++23
template<an_enum T>
constexpr bool
is_scoped_enum_v = not std::is_convertible_v<T, std::underlying_type_t<T>>;

template<typename T>
concept a_scoped_enum = is_scoped_enum_v<T>;


// detection concept

template<typename T>
constexpr bool
is_saturatingint_v = false;

template<a_scoped_enum E>
constexpr bool
is_saturatingint_v<E> = requires { E{} == E::psssatin_tag_to_prevent_mixing_other_enums; } ;

template<typename E>
using ULT=std::conditional_t<std::is_enum_v<plain<E>>,std::underlying_type_t<plain<E>>,plain<E>>;

template<typename E>
using promoted_t = // will promote keeping signedness
        std::conditional_t<(sizeof(ULT<E>) < sizeof(int))
            , std::conditional_t<std::is_unsigned_v<ULT<E>>
                , unsigned
                , int >
            , ULT<E>>;
}


template<typename E>
concept a_saturatingint = detail_::is_saturatingint_v<E>;

} // psssint


// provide std::numeric_limits
namespace std {



template<psssatin::a_saturatingint type>
  struct numeric_limits<type>
  {
    using ult = psssatin::detail_::ULT<type>;
    static constexpr bool is_specialized = true;

    static constexpr type
    min() noexcept { return type{numeric_limits<ult>::min()}; }

    static constexpr type
    max() noexcept { return type{numeric_limits<ult>::max()}; }

    static constexpr type
    lowest() noexcept { return type{numeric_limits<ult>::lowest()}; }

    static constexpr int digits = numeric_limits<ult>::digits;
    static constexpr int digits10 = numeric_limits<ult>::digits10;
    static constexpr int max_digits10 = numeric_limits<ult>::max_digits10;
    static constexpr bool is_signed = numeric_limits<ult>::is_signed;
    static constexpr bool is_integer = numeric_limits<ult>::is_integer;
    static constexpr bool is_exact = numeric_limits<ult>::is_exact;
    static constexpr int radix = numeric_limits<ult>::radix;

    static constexpr type
    epsilon() noexcept {  return type{numeric_limits<ult>::epsilon()}; }

    static constexpr type
    round_error() noexcept {  return type{numeric_limits<ult>::round_error()}; }

    static constexpr int min_exponent = numeric_limits<ult>::min_exponent;
    static constexpr int min_exponent10 = numeric_limits<ult>::min_exponent10;
    static constexpr int max_exponent = numeric_limits<ult>::max_exponent;
    static constexpr int max_exponent10 = numeric_limits<ult>::max_exponent10;

    static constexpr bool has_infinity = numeric_limits<ult>::has_infinity;
    static constexpr bool has_quiet_NaN = numeric_limits<ult>::has_quiet_NaN;
    static constexpr bool has_signaling_NaN = numeric_limits<ult>::has_signaling_NaN;
    static constexpr float_denorm_style has_denorm
     = numeric_limits<ult>::has_denorm;
    static constexpr bool has_denorm_loss = numeric_limits<ult>::has_denorm_loss;

    static constexpr type
    infinity() noexcept { return type{numeric_limits<ult>::infinity()}; }

    static constexpr type
    quiet_NaN() noexcept { return type{numeric_limits<ult>::quiet_NaN()}; }

    static constexpr type
    signaling_NaN() noexcept
    { return type{numeric_limits<ult>::signaling_NaN()}; }

    static constexpr type
    denorm_min() noexcept
    { return type{numeric_limits<ult>::denorm_min()}; }

    static constexpr bool is_iec559 =  numeric_limits<ult>::is_iec559;
    static constexpr bool is_bounded =  numeric_limits<ult>::is_bounded;
    static constexpr bool is_modulo =  true;

    static constexpr bool traps = false;
    static constexpr bool tinyness_before =  numeric_limits<ult>::tinyness_before;
    static constexpr float_round_style round_style =  numeric_limits<ult>::round_style;
  };

}

namespace psssatin{

namespace detail_{

template<a_saturatingint LEFT, a_saturatingint RIGHT>
constexpr bool
same_signedness_v = std::numeric_limits<LEFT>::is_signed == std::numeric_limits<RIGHT>::is_signed;

template<typename CHAR>
constexpr bool
is_chartype_v =    std::is_same_v<char,CHAR>
                || std::is_same_v<wchar_t,CHAR>
#ifdef __cpp_char8_t
                || std::is_same_v<char8_t,CHAR>
#endif
                || std::is_same_v<char16_t,CHAR>
                || std::is_same_v<char32_t,CHAR> ;



template<typename INT, typename TESTED>
constexpr bool
is_compatible_integer_v = std::is_same_v<TESTED,INT> ||
   (   std::is_integral_v<TESTED>
   && not std::is_same_v<bool,TESTED>
   && not is_chartype_v<TESTED>
   && (std::is_unsigned_v<INT> == std::is_unsigned_v<TESTED>)
   && std::numeric_limits<TESTED>::max() == std::numeric_limits<INT>::max() );


template<typename TESTED>
constexpr bool
is_known_integer_v =    is_compatible_integer_v<std::uint8_t,  TESTED>
                     || is_compatible_integer_v<std::uint16_t, TESTED>
                     || is_compatible_integer_v<std::uint32_t, TESTED>
                     || is_compatible_integer_v<std::uint64_t, TESTED>
                     || is_compatible_integer_v<std::int8_t,  TESTED>
                     || is_compatible_integer_v<std::int16_t, TESTED>
                     || is_compatible_integer_v<std::int32_t, TESTED>
                     || is_compatible_integer_v<std::int64_t, TESTED>;

}

template<typename LEFT, typename RIGHT>
concept same_signedness = detail_::same_signedness_v<LEFT,RIGHT>;

template<a_saturatingint E>
[[nodiscard]]
constexpr auto
promote_keep_signedness(E val) noexcept
{ // promote keeping signedness
    return static_cast<detail_::promoted_t<E>>(val);// promote with sign extension
}

// not used in framework:
template<a_saturatingint E>
[[nodiscard]]
constexpr auto
to_underlying(E val) noexcept 
{ // plain value with all bad properties
    return static_cast<std::underlying_type_t<E>>(val);
}

template<a_saturatingint E>
[[nodiscard]]
constexpr auto
promote_to_unsigned(E val) noexcept
{ // promote to unsigned for wrap around arithmetic
    using u_result_t = std::make_unsigned_t<detail_::promoted_t<E>>;
    return static_cast<u_result_t>(promote_keep_signedness(val));
}

// deliberately not std::integral, because of bool and characters!
template<typename T>
concept an_integer = detail_::is_known_integer_v<T>;

#ifdef __has_builtin
#if __has_builtin(__builtin_add_overflow)
# define HAVE_GCC_OVERFLOW_CHECKING
#endif
#endif
#if defined(__clang__)
# define HAVE_GCC_OVERFLOW_CHECKING
#endif

#ifndef __cpp_lib_saturation_arithmetic

// TODO: implement instead: https://locklessinc.com/articles/sat_arithmetic/
namespace non_builtin {
// like built-ins __builtin_add_overflow return true on overflow
template<an_integer T>
[[nodiscard]]
constexpr bool non_builtin_add_overflow(T l, T r, T* result) noexcept {
    if constexpr (std::numeric_limits<T>::is_signed){
        if constexpr(sizeof(T) == sizeof(std::int64_t)){
            *result = static_cast<T>(static_cast<uint64_t>(l) + static_cast<uint64_t>(r));
            if (l < 0){
                return (r<0) && (*result > l);
            } else {
                return (r >= 0) && (*result < l);
            }
        } else {
            std::int64_t res {l};
            res += r;
            *result = static_cast<T>(res);
            return res != *result;
        }
    } else { // unsigned
        if constexpr(sizeof(T) == sizeof(std::uint64_t)){
            *result = l + r;
            return *result < l; // wrapped when true
        } else {
            std::uint64_t res {l};
            res += r;
            *result = static_cast<T>(res);
            return res != *result;
        }
    }
    return true;
}
template<an_integer T>
[[nodiscard]]
constexpr bool non_builtin_sub_overflow(T l, T r, T* result) noexcept {
    if constexpr (std::numeric_limits<T>::is_signed){
        if constexpr(sizeof(T) == sizeof(std::int64_t)){
            *result = static_cast<T>(static_cast<uint64_t>(l) - static_cast<uint64_t>(r));
            if (l >= 0){
                return (r<0) && (*result < l);
            } else {
                return (r >= 0) && (*result > l);
            }
        } else {
            std::int64_t res {l};
            res -= r;
            *result = static_cast<T>(res);
            return res != *result;
        }
    } else { // unsigned
        if constexpr(sizeof(T) == sizeof(std::uint64_t)){
            *result = l - r;
            return *result > l; // wrapped when true
        } else {
            std::uint64_t res {l};
            res -= r;
            *result = static_cast<T>(res);
            return res != *result;
        }
    }
    return true;
}
template<an_integer T>
[[nodiscard]]
constexpr bool non_builtin_mul_overflow(T l, T r, T* result) noexcept {
    if constexpr (std::numeric_limits<T>::is_signed){
        if constexpr(sizeof(T) == sizeof(std::int64_t)){
            bool resultnegative { (l < 0) != (r < 0) };
            uint64_t res{};
            auto abs64 { [](int64_t val) -> uint64_t { return val < 0? 1ULL + ~static_cast<uint64_t>(val):static_cast<uint64_t>(val);} };
            if (not non_builtin_mul_overflow(abs64(l), abs64(r), &res) ){
                if (resultnegative) {
                    if (res <= static_cast<uint64_t>(std::numeric_limits<int64_t>::max())+1ull){
                        *result = static_cast<T>(1ULL + ~res); // two's complement
                        return false;
                    }
                } else {
                    if (res <= static_cast<uint64_t>(std::numeric_limits<int64_t>::max())) {
                        *result = static_cast<T>(res);
                        return false;
                    }
                }
            }
            return true; // overflow
        } else {
            std::int64_t res {l};
            res *= r;
            *result = static_cast<T>(res);
            return res != *result; // detect overflow bits
        }
    } else { // unsigned
        if constexpr(sizeof(T) == sizeof(std::uint64_t)){
            // compute high-parts and low-parts
            uint64_t lhigh { l >> 32 };
            uint64_t llow { l & 0xffff'ffffULL} ;
            uint64_t rhigh { r >> 32 };
            uint64_t rlow { r & 0xffff'ffffULL} ;
            if (lhigh > 0 && rhigh > 0) return true;
            uint64_t high_low{ lhigh>0? lhigh*rlow : rhigh*llow };
            if (high_low >> 32) return true; // overflow
            uint64_t low_low { llow * rlow } ;
            *result = (high_low << 32) + low_low;

            return *result < low_low; // detect overflow
        } else {
            std::uint64_t res {l};
            res *= r;
            *result = static_cast<T>(res);
            return res != *result;
        }
    }
    return true;
}
} // namespace non_builtin


#ifdef HAVE_GCC_OVERFLOW_CHECKING
template<an_integer T>
[[nodiscard]]
constexpr bool add_overflow(T l, T r, T* result) noexcept {
    return __builtin_add_overflow(l,r,result);
}
template<an_integer T>
[[nodiscard]]
constexpr bool sub_overflow(T l, T r, T* result) noexcept {
    return __builtin_sub_overflow(l,r,result);
}
template<an_integer T>
[[nodiscard]]
constexpr bool mul_overflow(T l, T r, T* result) noexcept {
    return __builtin_mul_overflow(l,r,result);
}

#else // DIY
template<an_integer T>
[[nodiscard]]
constexpr bool add_overflow(T l, T r, T* result) noexcept {
    return non_builtin::non_builtin_add_overflow(l,r,result);
}
template<an_integer T>
[[nodiscard]]
constexpr bool sub_overflow(T l, T r, T* result) noexcept {
    return non_builtin::non_builtin_sub_overflow(l,r,result);
}
template<an_integer T>
[[nodiscard]]
constexpr bool mul_overflow(T l, T r, T* result) noexcept {
    return non_builtin::non_builtin_mul_overflow(l,r,result);
}

#endif

#endif


template<an_integer TARGET, a_saturatingint E>
[[nodiscard]]
constexpr auto
promote_and_extend_to_unsigned(E val) noexcept
{ // promote to unsigned for wrap around arithmetic, with sign extension if needed
       using u_result_t = std::conditional_t< (sizeof(TARGET) > sizeof(detail_::promoted_t<E>)),
                std::make_unsigned_t<TARGET>, std::make_unsigned_t<detail_::promoted_t<E> > >;
       using s_result_t = std::make_signed_t<u_result_t>;
       return static_cast<u_result_t>(static_cast<s_result_t>(promote_keep_signedness(val)));// promote with sign extension
}


template<an_integer T>
[[nodiscard]]
constexpr auto
from_int(T val) noexcept {
    using detail_::is_compatible_integer_v;
    using std::conditional_t;
    struct cannot_convert_integer{};
    using result_t =
            conditional_t<is_compatible_integer_v<std::uint8_t,T>, sui8,
             conditional_t<is_compatible_integer_v<std::uint16_t,T>, sui16,
              conditional_t<is_compatible_integer_v<std::uint32_t,T>, sui32,
               conditional_t<is_compatible_integer_v<std::uint64_t,T>, sui64,
                conditional_t<is_compatible_integer_v<std::int8_t,T>, ssi8,
                 conditional_t<is_compatible_integer_v<std::int16_t,T>, ssi16,
                  conditional_t<is_compatible_integer_v<std::int32_t,T>, ssi32,
                   conditional_t<is_compatible_integer_v<std::int64_t,T>, ssi64, cannot_convert_integer>>>>>>>>;
    return static_cast<result_t>(val); // no need to check, result_t corresponds to input T's range
}
// path tests are compile-time checked:
template<a_saturatingint TO, an_integer FROM>
[[nodiscard]]
constexpr auto
from_int_to(FROM val) noexcept
{
    using result_t = TO;
    using ultr = std::underlying_type_t<result_t>;
    if constexpr(std::is_unsigned_v<ultr>){
        if constexpr(std::is_signed_v<FROM>){
            if (val < FROM{}){// 0
                return std::numeric_limits<result_t>::min(); // 0
            }
        }
    } else {
        if constexpr (std::is_signed_v<FROM>){
            if constexpr (sizeof(ultr) < sizeof(FROM)){
                if (val < static_cast<FROM>(std::numeric_limits<ultr>::min())) {
                    return std::numeric_limits<result_t>::min();
                }
            }
        }
    }
    if constexpr (sizeof(ultr) <= sizeof(FROM)){
        if (val > static_cast<FROM>(std::numeric_limits<ultr>::max())){
            return std::numeric_limits<result_t>::max();
        }
    }

    return static_cast<result_t>(val); // cast is checked above
}


// comparison
// not needed, we won't mix types in comparison.
// enum class types compare three way implicitly with themselves


// negation for signed types only, two's complement
template<a_saturatingint E>
constexpr E
operator-(E l) noexcept
requires std::numeric_limits<E>::is_signed
{
    if (l == std::numeric_limits<E>::min()) return std::numeric_limits<E>::max();
    return static_cast<E>(1u + ~promote_to_unsigned(l));
}


// increment/decrement

template<a_saturatingint E>
constexpr E&
operator++(E& l) noexcept
{
    return l = static_cast<E>(1) + l;
}

template<a_saturatingint E>
constexpr E
operator++(E& l, int) noexcept
{
    auto result=l;
    ++l;
    return result;
}
template<a_saturatingint E>
constexpr E&
operator--(E& l) noexcept
{
    return l = l - static_cast<E>(1);
}

template<a_saturatingint E>
constexpr E
operator--(E& l, int) noexcept
{
    auto result=l;
    --l;
    return result;
}



// arithmetic

template<a_saturatingint LEFT, a_saturatingint RIGHT>
constexpr auto
operator+(LEFT l, RIGHT r) noexcept
requires same_signedness<LEFT,RIGHT>
{
    // handle sign extension
    using result_t=std::conditional_t<sizeof(LEFT)>=sizeof(RIGHT),LEFT,RIGHT>;
    using ult = detail_::ULT<result_t>;
    ult result{};
    ult const left{static_cast<ult>(l)};
    ult const right{static_cast<ult>(r)};
#ifdef __cpp_lib_saturation_arithmetic
    result= std::add_sat<ult>(left,right) ;//+ !std::is_constant_evaluated(); // compilation check to force runtime error
#else
    if (add_overflow(left,right,&result)){
        if constexpr(std::numeric_limits<result_t>::is_signed){
            if (left < 0) return std::numeric_limits<result_t>::min();
        }
        return std::numeric_limits<result_t>::max();
    }
#endif
    return static_cast<result_t>(result);
}


template<a_saturatingint LEFT, a_saturatingint RIGHT>
constexpr auto&
operator+=(LEFT &l, RIGHT r) noexcept
requires same_signedness<LEFT,RIGHT>
{
    static_assert(sizeof(LEFT) >= sizeof(RIGHT),"psssatin: adding too large integer type");
    l = static_cast<LEFT>(l+r);
    return l;
}

template<a_saturatingint LEFT, a_saturatingint RIGHT>
constexpr auto
operator-(LEFT l, RIGHT r) noexcept
requires same_signedness<LEFT,RIGHT>
{
    using result_t=std::conditional_t<sizeof(LEFT)>=sizeof(RIGHT),LEFT,RIGHT>;
    using ult = detail_::ULT<result_t>;

    ult result{};
    ult const left{static_cast<ult>(l)};
    ult const right{static_cast<ult>(r)};
#ifdef __cpp_lib_saturation_arithmetic
    result = std::sub_sat<ult>(left,right);// + !std::is_constant_evaluated();
#else
    if (sub_overflow(left,right,&result)){
        if constexpr (std::numeric_limits<result_t>::is_signed) {
            if (right < 0) return std::numeric_limits<result_t>::max();
        }
        return std::numeric_limits<result_t>::min();
    }
#endif
    return static_cast<result_t>(result);
}
template<a_saturatingint LEFT, a_saturatingint RIGHT>
constexpr auto&
operator-=(LEFT &l, RIGHT r) noexcept
requires same_signedness<LEFT,RIGHT>
{
    static_assert(sizeof(LEFT) >= sizeof(RIGHT),"subtracting too large integer type");
    l = static_cast<LEFT>(l-r);
    return l;
}


template<a_saturatingint LEFT, a_saturatingint RIGHT>
constexpr auto
operator*(LEFT l, RIGHT r) noexcept
requires same_signedness<LEFT,RIGHT>
{
    using result_t=std::conditional_t<sizeof(LEFT)>=sizeof(RIGHT),LEFT,RIGHT>;
    using ult = detail_::ULT<result_t>;
    ult result{};
    ult const left{static_cast<ult>(l)};
    ult const right{static_cast<ult>(r)};
#ifdef __cpp_lib_saturation_arithmetic
    result = std::mul_sat<ult>(left,right);// + !std::is_constant_evaluated();
#else
    if (mul_overflow(left,right,&result)){
        if constexpr (std::numeric_limits<result_t>::is_signed){
            if ( (left < 0) != (right < 0) ) {
                return std::numeric_limits<result_t>::min();
            }
        }
        return std::numeric_limits<result_t>::max();
    }
#endif
    return static_cast<result_t>(result);
}
template<a_saturatingint LEFT, a_saturatingint RIGHT>
constexpr auto&
operator*=(LEFT &l, RIGHT r) noexcept
requires same_signedness<LEFT,RIGHT>
{
    static_assert(sizeof(LEFT) >= sizeof(RIGHT),"multiplying too large integer type");
    l = static_cast<LEFT>(l*r);
    return l;
}
template<a_saturatingint LEFT, a_saturatingint RIGHT>
constexpr auto
operator/(LEFT const l, RIGHT const r) noexcept
requires same_signedness<LEFT,RIGHT>
{
    using result_t=std::conditional_t<sizeof(LEFT)>=sizeof(RIGHT),LEFT,RIGHT>;
    using ult = detail_::ULT<result_t>;
    ult const numerator{static_cast<ult>(l)};
    ult const denominator{static_cast<ult>(r)};
    // using std::div_sat is not helpful, because it has UB with div by zero
    if constexpr (std::numeric_limits<result_t>::is_signed){
        // detect -min / -1 which is overflow
        if( numerator == std::numeric_limits<ult>::min() && denominator == -1){
            return std::numeric_limits<result_t>::min();
        }
        bool result_is_negative = (numerator < 0) != (denominator < 0);
        if (0 == denominator){
            if(result_is_negative)
                return std::numeric_limits<result_t>::min();
            else return std::numeric_limits<result_t>::max();
        }
    } else {
        if (0 == denominator) return std::numeric_limits<result_t>::max();
    }
    return static_cast<result_t>(numerator/denominator);

}
template<a_saturatingint LEFT, a_saturatingint RIGHT>
constexpr auto&
operator/=(LEFT &l, RIGHT r) noexcept
requires same_signedness<LEFT,RIGHT>
{
    static_assert(sizeof(LEFT) >= sizeof(RIGHT),"dividing by too large integer type");
    l = static_cast<LEFT>(l/r);
    return l;
}
template<a_saturatingint LEFT, a_saturatingint RIGHT>
constexpr auto
operator%(LEFT l, RIGHT r) noexcept
requires std::is_unsigned_v<detail_::ULT<LEFT>> && std::is_unsigned_v<detail_::ULT<RIGHT>>
{
    using result_t=std::conditional_t<sizeof(LEFT)>=sizeof(RIGHT),LEFT,RIGHT>;
    using ult = detail_::ULT<result_t>;
    if (r == RIGHT{}) return result_t{}; // return 0
    return static_cast<result_t>(
            static_cast<ult>(
                    promote_and_extend_to_unsigned<ult>(l)
                    % // unsigned modulo cannot overflow
                    promote_and_extend_to_unsigned<ult>(r)
            )
    );
}
template<a_saturatingint LEFT, a_saturatingint RIGHT>
constexpr auto&
operator%=(LEFT &l, RIGHT r) noexcept
requires std::is_unsigned_v<detail_::ULT<LEFT>> && std::is_unsigned_v<detail_::ULT<RIGHT>>
{
    static_assert(sizeof(LEFT) >= sizeof(RIGHT),"dividing by too large integer type");
    l = static_cast<LEFT>(l%r);
    return l;
}

// bitwise operators

template<a_saturatingint LEFT, a_saturatingint RIGHT>
constexpr auto
operator&(LEFT l, RIGHT r) noexcept
requires std::is_unsigned_v<detail_::ULT<LEFT>> && std::is_unsigned_v<detail_::ULT<RIGHT>>
{
    using result_t=std::conditional_t<sizeof(LEFT)>=sizeof(RIGHT),LEFT,RIGHT>;
    return static_cast<result_t>(promote_keep_signedness(l)&promote_keep_signedness(r));
}
template<a_saturatingint LEFT, a_saturatingint RIGHT>
constexpr auto&
operator&=(LEFT &l, RIGHT r) noexcept
requires std::is_unsigned_v<detail_::ULT<LEFT>> && std::is_unsigned_v<detail_::ULT<RIGHT>>
{
    static_assert(sizeof(LEFT) == sizeof(RIGHT),"bitand by different sized integer type");
    l = static_cast<LEFT>(l&r);
    return l;
}

template<a_saturatingint LEFT, a_saturatingint RIGHT>
constexpr auto
operator|(LEFT l, RIGHT r) noexcept
requires std::is_unsigned_v<detail_::ULT<LEFT>> && std::is_unsigned_v<detail_::ULT<RIGHT>>
{
    using result_t=std::conditional_t<sizeof(LEFT)>=sizeof(RIGHT),LEFT,RIGHT>;
    return static_cast<result_t>(promote_keep_signedness(l)|promote_keep_signedness(r));
}
template<a_saturatingint LEFT, a_saturatingint RIGHT>
constexpr auto&
operator|=(LEFT &l, RIGHT r) noexcept
requires std::is_unsigned_v<detail_::ULT<LEFT>> && std::is_unsigned_v<detail_::ULT<RIGHT>>
{
    static_assert(sizeof(LEFT) == sizeof(RIGHT),"bitor by different sized integer type");
    l = static_cast<LEFT>(l|r);
    return l;
}

template<a_saturatingint LEFT, a_saturatingint RIGHT>
constexpr auto
operator^(LEFT l, RIGHT r) noexcept
requires std::is_unsigned_v<detail_::ULT<LEFT>> && std::is_unsigned_v<detail_::ULT<RIGHT>>
{
    using result_t=std::conditional_t<sizeof(LEFT)>=sizeof(RIGHT),LEFT,RIGHT>;
    return static_cast<result_t>(promote_keep_signedness(l)^promote_keep_signedness(r));
}
template<a_saturatingint LEFT, a_saturatingint RIGHT>
constexpr auto&
operator^=(LEFT &l, RIGHT r) noexcept
requires std::is_unsigned_v<detail_::ULT<LEFT>> && std::is_unsigned_v<detail_::ULT<RIGHT>>
{
    static_assert(sizeof(LEFT) == sizeof(RIGHT),"xor by different sized integer type");
    l = static_cast<LEFT>(l^r);
    return l;
}

template<a_saturatingint LEFT>
constexpr LEFT
operator~(LEFT l) noexcept
requires std::is_unsigned_v<detail_::ULT<LEFT>>
{
    return static_cast<LEFT>(~promote_keep_signedness(l));
}


template<a_saturatingint LEFT, a_saturatingint RIGHT>
constexpr LEFT
operator<<(LEFT l, RIGHT r) noexcept
requires std::is_unsigned_v<detail_::ULT<LEFT>> && std::is_unsigned_v<detail_::ULT<RIGHT>>
{
    if( static_cast<size_t>(promote_keep_signedness(r)) < sizeof(LEFT)*CHAR_BIT){
        return static_cast<LEFT>(promote_keep_signedness(l)<<promote_keep_signedness(r));
    } else {
        return LEFT{}; // zero
    }
}
template<a_saturatingint LEFT, a_saturatingint RIGHT>
constexpr auto&
operator<<=(LEFT &l, RIGHT r) noexcept
requires std::is_unsigned_v<detail_::ULT<LEFT>> && std::is_unsigned_v<detail_::ULT<RIGHT>>
{
    l = (l<<r);
    return l;
}
template<a_saturatingint LEFT, a_saturatingint RIGHT>
constexpr LEFT
operator>>(LEFT l, RIGHT r) noexcept
requires std::is_unsigned_v<detail_::ULT<LEFT>> && std::is_unsigned_v<detail_::ULT<RIGHT>>
{
    if( static_cast<size_t>(promote_keep_signedness(r)) < sizeof(LEFT)*CHAR_BIT){
        return static_cast<LEFT>(promote_keep_signedness(l)>>promote_keep_signedness(r));
    } else {
        return LEFT{}; // zero
    }
}
template<a_saturatingint LEFT, a_saturatingint RIGHT>
constexpr auto&
operator>>=(LEFT &l, RIGHT r) noexcept
requires std::is_unsigned_v<detail_::ULT<LEFT>> && std::is_unsigned_v<detail_::ULT<RIGHT>>
{
    l = (l>>r);
    return l;
}


std::ostream& operator<<(std::ostream &out, a_saturatingint auto value){
    out << promote_keep_signedness(value);
    return out;
}

}

#endif /* SRC_PSSSATIN_ */
