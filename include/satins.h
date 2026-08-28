#ifndef PS_SATINS_H_
#define PS_SATINS_H_

#include <cstdint>
#include <type_traits>
#include <iosfwd>
#include <limits>
#include <climits>
#include <compare>
#ifdef __cpp_concepts
#include <concepts>
#endif
#if __cplusplus > 202302L
#include <numeric> // C++26 saturation arithmetic
#endif

namespace satins { // SATuration Integral Numbers with struct
namespace detail_ {
#if __cplusplus >= 202002L
template<typename T>
using plain = std::remove_cvref_t<T>;
#else
template<typename T>
using plain = std::remove_cv_t<std::remove_reference_t<T>>;
#endif

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
is_compatible_integer_v = std::is_same_v<plain<TESTED>,INT> ||
   (   std::is_integral_v<plain<TESTED>>
   && not std::is_same_v<bool,plain<TESTED>>
   && not is_chartype_v<plain<TESTED>>
   && (std::is_unsigned_v<INT> == std::is_unsigned_v<TESTED>)
   && std::numeric_limits<plain<TESTED>>::max() == std::numeric_limits<INT>::max()
   );

template<typename TESTED,typename=void>
constexpr bool
is_known_integer_v =false;
// only support the following sizes:
template<typename TESTED>
constexpr bool
is_known_integer_v<TESTED,std::enable_if_t<std::is_integral_v<TESTED>>> =
                        is_compatible_integer_v<std::uint8_t,  TESTED>
                     || is_compatible_integer_v<std::uint16_t, TESTED>
                     || is_compatible_integer_v<std::uint32_t, TESTED>
                     || is_compatible_integer_v<std::uint64_t, TESTED>
                     || is_compatible_integer_v<std::int8_t,  TESTED>
                     || is_compatible_integer_v<std::int16_t, TESTED>
                     || is_compatible_integer_v<std::int32_t, TESTED>
                     || is_compatible_integer_v<std::int64_t, TESTED>;
}
#ifdef __cpp_concepts
template<typename TESTED>
concept sized_integer = detail_::is_known_integer_v<TESTED>;
#endif
// deliberately not std::integral, because of bool and characters!
namespace detail_ {

#ifdef __has_builtin
#if __has_builtin(__builtin_add_overflow)
# define HAVE_GCC_OVERFLOW_CHECKING
#endif
#endif
#if defined(__clang__)
# define HAVE_GCC_OVERFLOW_CHECKING
#endif

#ifndef __cpp_lib_saturation_arithmetic

namespace non_builtin {
// like built-ins __builtin_add_overflow return true on overflow
#ifdef __cpp_concepts
template<sized_integer T>
#else
template<typename T>
#endif
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
#ifdef __cpp_concepts
template<sized_integer T>
#else
template<typename T>
#endif
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
#ifdef __cpp_concepts
template<sized_integer T>
#else
template<typename T>
#endif
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
#ifdef __cpp_concepts
template<sized_integer T>
#else
template<typename T>
#endif
[[nodiscard]]
constexpr bool add_overflow(T l, T r, T* result) noexcept {
    return __builtin_add_overflow(l,r,result);
}
#ifdef __cpp_concepts
template<sized_integer T>
#else
template<typename T>
#endif
[[nodiscard]]
constexpr bool sub_overflow(T l, T r, T* result) noexcept {
    return __builtin_sub_overflow(l,r,result);
}
#ifdef __cpp_concepts
template<sized_integer T>
#else
template<typename T>
#endif
[[nodiscard]]
constexpr bool mul_overflow(T l, T r, T* result) noexcept {
    return __builtin_mul_overflow(l,r,result);
}

#else // DIY
#ifdef __cpp_concepts
template<sized_integer T>
#else
template<typename T>
#endif
[[nodiscard]]
constexpr bool add_overflow(T l, T r, T* result) noexcept {
    return detail_::non_builtin::non_builtin_add_overflow(l,r,result);
}
#ifdef __cpp_concepts
template<sized_integer T>
#else
template<typename T>
#endif
[[nodiscard]]
constexpr bool sub_overflow(T l, T r, T* result) noexcept {
    return detail_::non_builtin::non_builtin_sub_overflow(l,r,result);
}
#ifdef __cpp_concepts
template<sized_integer T>
#else
template<typename T>
#endif
[[nodiscard]]
constexpr bool mul_overflow(T l, T r, T* result) noexcept {
    return detail_::non_builtin::non_builtin_mul_overflow(l,r,result);
}

#endif


#endif



} // NS detail_


#ifdef __cpp_concepts
template<sized_integer INT>
#else
template<typename INT, typename=std::enable_if_t<detail_::is_known_integer_v<INT>>>
#endif
struct [[nodiscard]] Satin;

namespace detail_{
template<typename T, typename=void>
constexpr bool is_saturatingint_v = false;
#ifdef __cpp_concepts
template<sized_integer I>
constexpr bool is_saturatingint_v<Satin<I>> = true;
#else
template<typename INT, typename VOID=std::enable_if_t<detail_::is_known_integer_v<INT>>>
constexpr bool is_saturatingint_v<Satin<INT,VOID>> = true;
#endif

template<typename C>
struct ULT_impl;
template<typename I>
struct ULT_impl<Satin<I>>{
    using type=I;
};
} // NS detail_
#ifdef __cpp_concepts
template<typename E>
concept a_saturatingint = detail_::is_saturatingint_v<E>;
#endif

template<typename C>
using ULT=typename detail_::ULT_impl<detail_::plain<C>>::type;


template<typename E>
using promoted_t = // will promote the underlying type keeping signedness
        std::conditional_t<(sizeof(ULT<E>) < sizeof(int))
            , std::conditional_t<std::is_unsigned_v<ULT<E>>
                , unsigned
                , int >
            , ULT<E>>;
#ifdef __cpp_concepts
template<a_saturatingint E>
#else
template<typename E, typename=std::enable_if_t<detail_::is_saturatingint_v<E>>>
#endif
[[nodiscard]]
constexpr auto
promote_keep_signedness(E value) noexcept
{ // promote keeping signedness
    return static_cast<promoted_t<E>>(static_cast<ULT<E>>(value));// promote with sign extension
}
namespace detail_ {


#ifdef __cpp_concepts
template<a_saturatingint E>
#else
template<typename E, typename=std::enable_if_t<detail_::is_saturatingint_v<E>>>
#endif
[[nodiscard]]
constexpr auto
promote_to_unsigned(E value) noexcept
{ // promote to unsigned for wrap around arithmetic
    using u_result_t = std::make_unsigned_t<promoted_t<E>>;
    return static_cast<u_result_t>(promote_keep_signedness(value));
}
#ifdef __cpp_concepts
template<sized_integer TARGET, a_saturatingint E>
#else
template<typename TARGET, typename E, typename=std::enable_if_t<detail_::is_known_integer_v<TARGET> && detail_::is_saturatingint_v<E>>>
#endif
[[nodiscard]]
constexpr auto
promote_and_extend_to_unsigned(E value) noexcept
{ // promote to unsigned for wrap around arithmetic, with sign extension if needed
       using u_result_t = std::conditional_t< (sizeof(TARGET) > sizeof(promoted_t<E>)),
                std::make_unsigned_t<TARGET>, std::make_unsigned_t<promoted_t<E> > >;
       using s_result_t = std::make_signed_t<u_result_t>;
       return static_cast<u_result_t>(static_cast<s_result_t>(promote_keep_signedness(value)));// promote with sign extension
}

#ifdef __cpp_concepts
template<sized_integer result_t, sized_integer FROM>
#else
template<typename result_t, typename FROM,
typename=std::enable_if_t<detail_::is_known_integer_v<result_t> && detail_::is_known_integer_v<FROM>>>
#endif
[[nodiscard]]
constexpr auto
from_int_to_impl(FROM value)
{
#ifdef __cpp_lib_saturation_arithmetic
    return static_cast<result_t>(std::saturate_cast<result_t>(value));
#else
   if constexpr(not std::is_same_v<result_t,FROM>) {
        if constexpr (std::is_unsigned_v<result_t>){
            if constexpr (std::is_signed_v<FROM>){
                if (value < FROM{}){
                    return result_t{}; // zero
                }// value is positive below
            }
            if (static_cast<uint64_t>(value) > std::numeric_limits<result_t>::max()) {
                return std::numeric_limits<result_t>::max();
            }
        } else { // to signed
            if constexpr (std::is_signed_v<FROM>) {
                if (static_cast<int64_t>(value) < std::numeric_limits<result_t>::min()) {
                    return std::numeric_limits<result_t>::min();
                }
            }
            if (value>0 && static_cast<uint64_t>(value) > std::numeric_limits<result_t>::max()) {
                return std::numeric_limits<result_t>::max();
            }
        }
    }
    return static_cast<result_t>(static_cast<result_t>(value)); // value is checked above
#endif
}


} // NS detail_

template<typename LEFT, typename RIGHT>
constexpr bool
same_signedness_v = std::numeric_limits<LEFT>::is_signed == std::numeric_limits<RIGHT>::is_signed;

#ifdef __cpp_concepts
template<typename LEFT, typename RIGHT>
concept same_signedness = same_signedness_v<LEFT,RIGHT>;
#endif

// path tests are compile-time checked:
#ifdef __cpp_concepts
template<a_saturatingint TO, sized_integer FROM>
#else
template<typename TO, typename FROM, typename=std::enable_if_t<detail_::is_known_integer_v<FROM> && detail_::is_saturatingint_v<TO>>>
#endif
[[nodiscard]]
constexpr auto
from_int_to(FROM value) noexcept
{
   return static_cast<TO>(detail_::from_int_to_impl<ULT<TO>>(value));
}

#ifdef __cpp_concepts
template<sized_integer INT>
#else
template<typename INT, typename>
#endif
struct [[nodiscard]] Satin{
    constexpr Satin() noexcept:value_which_should_not_be_referred_to_from_user_code{}{}
#if __cplusplus >= 202002L
    explicit constexpr Satin(std::same_as<INT> auto v) noexcept:value_which_should_not_be_referred_to_from_user_code(v){
    }
    friend constexpr auto operator<=>(Satin, Satin) noexcept = default;
    template<sized_integer T>
    explicit constexpr Satin(T v)
    requires (not std::same_as<INT,detail_::plain<T>>)
    :value_which_should_not_be_referred_to_from_user_code(from_int_to<Satin>(v)){}
#else
    template<typename T, typename=std::enable_if_t<detail_::is_known_integer_v<T>>>
    explicit constexpr Satin(T v) noexcept
    :value_which_should_not_be_referred_to_from_user_code{detail_::from_int_to_impl<INT>(v)}{}
    friend constexpr bool operator<(Satin left, Satin right) {
        return left.value_which_should_not_be_referred_to_from_user_code < right.value_which_should_not_be_referred_to_from_user_code;
    }
    friend constexpr bool operator>=(Satin left, Satin right) {
        return not (left < right);
    }
    friend constexpr bool operator>(Satin left, Satin right) {
        return right < left;
    }
    friend constexpr bool operator<=(Satin left, Satin right) {
        return not (left > right);
    }
    friend constexpr bool operator==(Satin left, Satin right) {
        return left.value_which_should_not_be_referred_to_from_user_code == right.value_which_should_not_be_referred_to_from_user_code;
    }
    friend constexpr bool operator!=(Satin left, Satin right) {
        return not (left==right);
    }
#endif
    // member/friend operators
    explicit constexpr operator INT() const noexcept { return value_which_should_not_be_referred_to_from_user_code;}

    // negation for signed types only, two's complement
    template<typename T=INT>
    constexpr auto
    operator-() const noexcept
#ifdef __cpp_concepts
    requires std::numeric_limits<INT>::is_signed
#else
    -> std::enable_if_t<std::numeric_limits<T>::is_signed,Satin>
#endif
    {
        if (value_which_should_not_be_referred_to_from_user_code == std::numeric_limits<INT>::min())
              return std::numeric_limits<Satin>::max(); // prevent wrapping
        return Satin(static_cast<INT>(-value_which_should_not_be_referred_to_from_user_code));
    }

    // increment/decrement

    constexpr Satin& operator++() & noexcept
    {
        return *this = static_cast<Satin>(1) + *this;
    }

    constexpr Satin operator++(int) & noexcept
    {
        auto result=*this;
        ++ *this;
        return result;
    }
    constexpr Satin& operator--() & noexcept
    {
        return *this = *this - static_cast<Satin>(1);
    }

    constexpr Satin operator--(int) & noexcept
    {
        auto result=*this;
        --*this;
        return result;
    }

    // arithmetic
#ifdef __cpp_concepts
    template<a_saturatingint RIGHT>
#else
    template<typename RIGHT, typename=std::enable_if_t<
      detail_::is_saturatingint_v<RIGHT> &&
      same_signedness_v<Satin,RIGHT>>>
#endif
    friend constexpr auto
    operator+(Satin l, RIGHT r) noexcept
#ifdef __cpp_concepts
    requires same_signedness<Satin,RIGHT>
#endif
    {
        // handle sign extension
        using result_t=std::conditional_t<sizeof(Satin)>=sizeof(RIGHT),Satin,RIGHT>;
        using ult = ULT<result_t>;
        ult result{};
        ult const left{static_cast<ult>(static_cast<ULT<Satin>>(l))};
        ult const right{static_cast<ult>(static_cast<ULT<RIGHT>>(r))};
    #ifdef __cpp_lib_saturation_arithmetic
        result= std::add_sat<ult>(left,right) ;//+ !std::is_constant_evaluated(); // compilation check to force runtime error
    #else
        if (detail_::add_overflow(left,right,&result)){
            if constexpr(std::numeric_limits<result_t>::is_signed){
                if (left < 0) return std::numeric_limits<result_t>::min();
            }
            return std::numeric_limits<result_t>::max();
        }
    #endif
        return static_cast<result_t>(result);
    }


#ifdef __cpp_concepts
    template<a_saturatingint RIGHT>
#else
    template<typename RIGHT, typename=std::enable_if_t<
      detail_::is_saturatingint_v<RIGHT> &&
      same_signedness_v<Satin,RIGHT>>>
#endif
    constexpr auto&
    operator+=(RIGHT r)  & noexcept
#ifdef __cpp_concepts
    requires same_signedness<Satin,RIGHT>
#endif
    {
        static_assert(sizeof(Satin) >= sizeof(RIGHT),"satins: adding too large integer type");
        *this = static_cast<Satin>(*this+r);
        return *this;
    }

#ifdef __cpp_concepts
    template<a_saturatingint RIGHT>
#else
    template<typename RIGHT, typename=std::enable_if_t<
      detail_::is_saturatingint_v<RIGHT> &&
      same_signedness_v<Satin,RIGHT>>>
#endif
    friend constexpr auto
    operator-(Satin l, RIGHT r) noexcept
#ifdef __cpp_concepts
    requires same_signedness<Satin,RIGHT>
#endif
    {
        using result_t=std::conditional_t<sizeof(Satin)>=sizeof(RIGHT),Satin,RIGHT>;
        using ult = ULT<result_t>;

        ult result{};
        ult const left{static_cast<ult>(static_cast<ULT<Satin>>(l))};
        ult const right{static_cast<ult>(static_cast<ULT<RIGHT>>(r))};
    #ifdef __cpp_lib_saturation_arithmetic
        result = std::sub_sat<ult>(left,right);// + !std::is_constant_evaluated();
    #else
        if (detail_::sub_overflow(left,right,&result)){
            if constexpr (std::numeric_limits<result_t>::is_signed) {
                if (right < 0) return std::numeric_limits<result_t>::max();
            }
            return std::numeric_limits<result_t>::min();
        }
    #endif
        return static_cast<result_t>(result);
    }
#ifdef __cpp_concepts
    template<a_saturatingint RIGHT>
#else
    template<typename RIGHT, typename=std::enable_if_t<
      detail_::is_saturatingint_v<RIGHT> &&
      same_signedness_v<Satin,RIGHT>>>
#endif
    constexpr auto&
    operator-=(RIGHT r) & noexcept
#ifdef __cpp_concepts
    requires same_signedness<Satin,RIGHT>
#endif
    {
        static_assert(sizeof(Satin) >= sizeof(RIGHT),"subtracting too large integer type");
        *this = static_cast<Satin>(*this-r);
        return *this;
    }


#ifdef __cpp_concepts
    template<a_saturatingint RIGHT>
#else
    template<typename RIGHT, typename=std::enable_if_t<
      detail_::is_saturatingint_v<RIGHT> &&
      same_signedness_v<Satin,RIGHT>>>
#endif
    friend constexpr auto
    operator*(Satin l, RIGHT r) noexcept
#ifdef __cpp_concepts
    requires same_signedness<Satin,RIGHT>
#endif
    {
        using result_t=std::conditional_t<sizeof(Satin)>=sizeof(RIGHT),Satin,RIGHT>;
        using ult = ULT<result_t>;
        ult result{};
        ult const left{static_cast<ult>(static_cast<ULT<Satin>>(l))};
        ult const right{static_cast<ult>(static_cast<ULT<RIGHT>>(r))};
    #ifdef __cpp_lib_saturation_arithmetic
        result = std::mul_sat<ult>(left,right);// + !std::is_constant_evaluated();
    #else
        if (detail_::mul_overflow(left,right,&result)){
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
#ifdef __cpp_concepts
    template<sized_integer RIGHT>
#else
    template<typename RIGHT>
#endif
    friend constexpr auto
    operator*(Satin l, RIGHT r)
#ifndef __cpp_concepts
    ->std::enable_if_t<detail_::is_known_integer_v<RIGHT>,Satin>
#endif
    {
        return l * from_int_to<Satin>(r);
    }
#ifdef __cpp_concepts
    template<sized_integer LEFT>
#else
    template<typename LEFT>
#endif
    friend constexpr auto
    operator*(LEFT l, Satin r)
#ifndef __cpp_concepts
    ->std::enable_if_t<detail_::is_known_integer_v<LEFT>,Satin>
#endif
    {
        return from_int_to<Satin>(l) * r;
    }
#ifdef __cpp_concepts
    template<a_saturatingint RIGHT>
#else
    template<typename RIGHT, typename=std::enable_if_t<
      detail_::is_saturatingint_v<RIGHT> &&
      same_signedness_v<Satin,RIGHT>>>
#endif
    constexpr auto&
    operator*=(RIGHT r) & noexcept
#ifdef __cpp_concepts
    requires same_signedness<Satin,RIGHT>
#endif
    {
        static_assert(sizeof(Satin) >= sizeof(RIGHT),"multiplying too large integer type");
        *this = static_cast<Satin>(*this*r);
        return *this;
    }
#ifdef __cpp_concepts
    template<sized_integer RIGHT>
    constexpr auto&
#else
    template<typename RIGHT>
    constexpr auto
#endif
    operator*=(RIGHT r) &
#ifndef __cpp_concepts
    ->std::enable_if_t<detail_::is_known_integer_v<RIGHT>,Satin&>
#endif
    {
        return *this *= from_int_to<Satin>(r);
    }
#ifdef __cpp_concepts
    template<a_saturatingint RIGHT>
#else
    template<typename RIGHT, typename=std::enable_if_t<
      detail_::is_saturatingint_v<RIGHT> &&
      same_signedness_v<Satin,RIGHT>>>
#endif
    friend constexpr auto
    operator/(Satin const l, RIGHT const r) noexcept
#ifdef __cpp_concepts
    requires same_signedness<Satin,RIGHT>
#endif
    {
        using result_t=std::conditional_t<sizeof(Satin)>=sizeof(RIGHT),Satin,RIGHT>;
        //integral promotion is a bitch
        using ult = ULT<result_t>;
        ult const numerator{static_cast<ult>(static_cast<ULT<Satin>>(l))};
        ult const denominator{static_cast<ult>(static_cast<ULT<RIGHT>>(r))};
        // using std::div_sat is not helpful, because it has UB with div by zero
        if constexpr (std::numeric_limits<result_t>::is_signed){
            // detect -min / -1 which is overflow
            if( numerator == std::numeric_limits<ult>::min() && denominator == -1){
                return std::numeric_limits<result_t>::max();
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
        return static_cast<result_t>(static_cast<ult>(numerator/denominator));
    }
#ifdef __cpp_concepts
    template<sized_integer RIGHT>
#else
    template<typename RIGHT>
#endif
    friend constexpr auto
    operator/(Satin const l, RIGHT const r)
#ifndef __cpp_concepts
    ->std::enable_if_t<detail_::is_known_integer_v<RIGHT>,Satin>
#endif
    {
        return l / from_int_to<Satin>(r);
    }
#ifdef __cpp_concepts
    template<a_saturatingint RIGHT>
#else
    template<typename RIGHT, typename=std::enable_if_t<
      detail_::is_saturatingint_v<RIGHT> &&
      same_signedness_v<Satin,RIGHT>>>
#endif
    constexpr auto&
    operator/=(RIGHT r) & noexcept
#ifdef __cpp_concepts
    requires same_signedness<Satin,RIGHT>
#endif
    {
        static_assert(sizeof(Satin) >= sizeof(RIGHT),"dividing by too large integer type");
        *this = static_cast<Satin>(*this/r);
        return *this;
    }
#ifdef __cpp_concepts
    template<sized_integer RIGHT>
    constexpr auto&
#else
    template<typename RIGHT>
    constexpr auto
#endif
    operator/=(RIGHT r) &
#ifndef __cpp_concepts
    ->std::enable_if_t<detail_::is_known_integer_v<RIGHT>,Satin&>
#endif
    {
        return *this /= from_int_to<Satin>(r);
    }
#ifdef __cpp_concepts
    template<a_saturatingint RIGHT>
#else
    template<typename RIGHT, typename=std::enable_if_t<
      detail_::is_saturatingint_v<RIGHT> &&
      std::is_unsigned_v<ULT<Satin>> &&
      std::is_unsigned_v<ULT<RIGHT>>>>
#endif
    friend constexpr auto
    operator%(Satin l, RIGHT r) noexcept
#ifdef __cpp_concepts
    requires std::is_unsigned_v<ULT<Satin>> && std::is_unsigned_v<ULT<RIGHT>>
#endif
    {
        using result_t=std::conditional_t<sizeof(Satin)>=sizeof(RIGHT),Satin,RIGHT>;
        using ult = ULT<result_t>;
        if (r == RIGHT{}) return result_t{}; // return 0
        return static_cast<result_t>(
                static_cast<ult>(
                        detail_::promote_and_extend_to_unsigned<ult>(l)
                        % // unsigned modulo cannot overflow
                        detail_::promote_and_extend_to_unsigned<ult>(r)
                )
        );
    }
#ifdef __cpp_concepts
    template<a_saturatingint RIGHT>
#else
    template<typename RIGHT, typename=std::enable_if_t<
      detail_::is_saturatingint_v<RIGHT> &&
      std::is_unsigned_v<ULT<Satin>> &&
      std::is_unsigned_v<ULT<RIGHT>>>>
#endif
    constexpr auto&
    operator%=(RIGHT r) & noexcept
#ifdef __cpp_concepts
    requires std::is_unsigned_v<ULT<Satin>> && std::is_unsigned_v<ULT<RIGHT>>
#endif
    {
        static_assert(sizeof(Satin) >= sizeof(RIGHT),"dividing by too large integer type");
        *this = static_cast<Satin>(*this % r);
        return *this;
    }
    // bitwise operators

#ifdef __cpp_concepts
    template<a_saturatingint RIGHT>
#else
    template<typename RIGHT, typename=std::enable_if_t<
      detail_::is_saturatingint_v<RIGHT> &&
      std::is_unsigned_v<ULT<Satin>> &&
      std::is_unsigned_v<ULT<RIGHT>>>>
#endif
    friend constexpr auto
    operator&(Satin l, RIGHT r) noexcept
#ifdef __cpp_concepts
    requires std::is_unsigned_v<ULT<Satin>> && std::is_unsigned_v<ULT<RIGHT>>
#endif
    {
        using result_t=std::conditional_t<sizeof(Satin)>=sizeof(RIGHT),Satin,RIGHT>;
        return static_cast<result_t>(promote_keep_signedness(l)&promote_keep_signedness(r));
    }
#ifdef __cpp_concepts
    template<a_saturatingint RIGHT>
#else
    template<typename RIGHT, typename=std::enable_if_t<
      detail_::is_saturatingint_v<RIGHT> &&
      std::is_unsigned_v<ULT<Satin>> &&
      std::is_unsigned_v<ULT<RIGHT>>>>
#endif
    constexpr auto&
    operator&=(RIGHT r) & noexcept
#ifdef __cpp_concepts
    requires std::is_unsigned_v<ULT<Satin>> && std::is_unsigned_v<ULT<RIGHT>>
#endif
    {
        static_assert(sizeof(Satin) == sizeof(RIGHT),"bitand by different sized integer type");
        *this = static_cast<Satin>(*this&r);
        return *this;
    }

#ifdef __cpp_concepts
    template<a_saturatingint RIGHT>
#else
    template<typename RIGHT, typename=std::enable_if_t<
      detail_::is_saturatingint_v<RIGHT> &&
      std::is_unsigned_v<ULT<Satin>> &&
      std::is_unsigned_v<ULT<RIGHT>>>>
#endif
    friend constexpr auto
    operator|(Satin l, RIGHT r) noexcept
#ifdef __cpp_concepts
    requires std::is_unsigned_v<ULT<Satin>> && std::is_unsigned_v<ULT<RIGHT>>
#endif
    {
        using result_t=std::conditional_t<sizeof(Satin)>=sizeof(RIGHT),Satin,RIGHT>;
        return static_cast<result_t>(promote_keep_signedness(l)|promote_keep_signedness(r));
    }
#ifdef __cpp_concepts
    template<a_saturatingint RIGHT>
#else
    template<typename RIGHT, typename=std::enable_if_t<
      detail_::is_saturatingint_v<RIGHT> &&
      std::is_unsigned_v<ULT<Satin>> &&
      std::is_unsigned_v<ULT<RIGHT>>>>
#endif
    constexpr auto&
    operator|=(RIGHT r) & noexcept
#ifdef __cpp_concepts
    requires std::is_unsigned_v<ULT<Satin>> && std::is_unsigned_v<ULT<RIGHT>>
#endif
    {
        static_assert(sizeof(Satin) == sizeof(RIGHT),"bitor by different sized integer type");
        *this = static_cast<Satin>(*this|r);
        return *this;
    }

#ifdef __cpp_concepts
    template<a_saturatingint RIGHT>
#else
    template<typename RIGHT, typename=std::enable_if_t<
      detail_::is_saturatingint_v<RIGHT> &&
      std::is_unsigned_v<ULT<Satin>> &&
      std::is_unsigned_v<ULT<RIGHT>>>>
#endif
    friend constexpr auto
    operator^(Satin l, RIGHT r) noexcept
#ifdef __cpp_concepts
    requires std::is_unsigned_v<ULT<Satin>> && std::is_unsigned_v<ULT<RIGHT>>
#endif
    {
        using result_t=std::conditional_t<sizeof(Satin)>=sizeof(RIGHT),Satin,RIGHT>;
        return static_cast<result_t>(promote_keep_signedness(l)^promote_keep_signedness(r));
    }
#ifdef __cpp_concepts
    template<a_saturatingint RIGHT>
#else
    template<typename RIGHT, typename=std::enable_if_t<
      detail_::is_saturatingint_v<RIGHT> &&
      std::is_unsigned_v<ULT<Satin>> &&
      std::is_unsigned_v<ULT<RIGHT>>>>
#endif
    constexpr auto&
    operator^=(RIGHT r) & noexcept
#ifdef __cpp_concepts
    requires std::is_unsigned_v<ULT<Satin>> && std::is_unsigned_v<ULT<RIGHT>>
#endif
    {
        static_assert(sizeof(Satin) == sizeof(RIGHT),"xor by different sized integer type");
        *this = static_cast<Satin>(*this^r);
        return *this;
    }

    template<typename ult=ULT<Satin>>
    friend constexpr auto
    operator~(Satin l) noexcept
#ifdef __cpp_concpets
    requires std::is_unsigned_v<ULT<Satin>>
#else
    ->std::enable_if_t<std::is_unsigned_v<ult>,Satin>
#endif
    {
        return static_cast<Satin>(static_cast<ULT<Satin>>(~promote_keep_signedness(l)));
    }


#ifdef __cpp_concepts
    template<a_saturatingint RIGHT>
#else
    template<typename RIGHT, typename=std::enable_if_t<
      detail_::is_saturatingint_v<RIGHT> &&
      std::is_unsigned_v<ULT<Satin>> &&
      std::is_unsigned_v<ULT<RIGHT>>>>
#endif
    friend constexpr Satin
    operator<<(Satin l, RIGHT r) noexcept
#ifdef __cpp_concepts
    requires std::is_unsigned_v<ULT<Satin>> && std::is_unsigned_v<ULT<RIGHT>>
#endif
    {
        if( static_cast<size_t>(promote_keep_signedness(r)) < sizeof(Satin)*CHAR_BIT){
            return static_cast<Satin>(promote_keep_signedness(l)<<promote_keep_signedness(r));
        } else {
            return Satin{}; // zero
        }
    }
#ifdef __cpp_concepts
    template<a_saturatingint RIGHT>
#else
    template<typename RIGHT, typename=std::enable_if_t<
      detail_::is_saturatingint_v<RIGHT> &&
      std::is_unsigned_v<ULT<Satin>> &&
      std::is_unsigned_v<ULT<RIGHT>>>>
#endif
    constexpr auto&
    operator<<=(RIGHT r) & noexcept
#ifdef __cpp_concepts
    requires std::is_unsigned_v<ULT<Satin>> && std::is_unsigned_v<ULT<RIGHT>>
#endif
    {
        *this = (*this<<r);
        return *this;
    }
#ifdef __cpp_concepts
    template<a_saturatingint RIGHT>
#else
    template<typename RIGHT, typename=std::enable_if_t<
      detail_::is_saturatingint_v<RIGHT> &&
      std::is_unsigned_v<ULT<Satin>> &&
      std::is_unsigned_v<ULT<RIGHT>>>>
#endif
    friend constexpr Satin
    operator>>(Satin l, RIGHT r) noexcept
#ifdef __cpp_concepts
    requires std::is_unsigned_v<ULT<Satin>> && std::is_unsigned_v<ULT<RIGHT>>
#endif
    {
        if( static_cast<size_t>(promote_keep_signedness(r)) < sizeof(Satin)*CHAR_BIT){
            return static_cast<Satin>(
                    promote_keep_signedness(l)>>promote_keep_signedness(r));
        } else {
            return Satin{}; // zero
        }
    }
#ifdef __cpp_concepts
    template<a_saturatingint RIGHT>
#else
    template<typename RIGHT, typename=std::enable_if_t<
      detail_::is_saturatingint_v<RIGHT> &&
      std::is_unsigned_v<ULT<Satin>> &&
      std::is_unsigned_v<ULT<RIGHT>>>>
#endif
    constexpr auto&
    operator>>=( RIGHT r) & noexcept
#ifdef __cpp_concepts
    requires std::is_unsigned_v<ULT<Satin>> && std::is_unsigned_v<ULT<RIGHT>>
#endif
    {
        *this = (*this >>r);
        return *this;
    }


    friend std::ostream& operator<<(std::ostream &out, Satin value){
        out << promote_keep_signedness(value);
        return out;
    }
    // no need for private, makes compilability checks possible
    // all possible values of INT are valid
    INT value_which_should_not_be_referred_to_from_user_code;
};
// unsigned

template<typename T>
constexpr auto to_underlying(T val){
    return static_cast<ULT<T>>(val);
}


using sui8  = Satin<std::uint8_t >;
using sui16 = Satin<std::uint16_t>;
using sui32 = Satin<std::uint32_t>;
using sui64 = Satin<std::uint64_t>;
#if __cplusplus == 201703L
#define CONSTEVAL constexpr
#elif __cplusplus >= 202002L
#define CONSTEVAL consteval
#else
#error "requires at least C++17, best C++20"
#endif
inline namespace literals {
CONSTEVAL
sui8 operator""_sui8(unsigned long long value) {
    if (value <= std::numeric_limits<std::uint8_t>::max()) {
        return sui8(static_cast<std::uint8_t>(value));
    } else {
        throw "integral constant too large"; // trigger compile-time error
    }
}


CONSTEVAL
sui16 operator""_sui16(unsigned long long value) {
    if (value <= std::numeric_limits<std::uint16_t>::max()) {
        return sui16(static_cast<std::uint16_t>(value));
    } else {
        throw "integral constant too large"; // trigger compile-time error
    }
}


CONSTEVAL
sui32 operator""_sui32(unsigned long long value) {
    if (value <= std::numeric_limits<uint32_t>::max()) {
        return sui32(static_cast<std::uint32_t>(value));
    } else {
        throw "integral constant too large"; // trigger compile-time error
    }
}


CONSTEVAL
sui64 operator""_sui64(unsigned long long value) {
    if constexpr (sizeof(sui64) < sizeof(value)){
        if (value > 0xffff'ffff'fffffffful) {
            throw "integral constant too large"; // trigger compile-time error
        }
    }
    return sui64(static_cast<std::uint64_t>(value));
}

}

// signed
using ssi8  = Satin<std::int8_t >;
using ssi16 = Satin<std::int16_t>;
using ssi32 = Satin<std::int32_t>;
using ssi64 = Satin<std::int64_t>;


inline namespace literals {
CONSTEVAL
ssi8 operator""_ssi8(unsigned long long value) {
    if (value <= std::numeric_limits<std::int8_t>::max()) {
        return ssi8(static_cast<int8_t>(value));
    } else {
        throw "integral constant too large"; // trigger compile-time error
    }
}


CONSTEVAL
ssi16 operator""_ssi16(unsigned long long value) {
    if (value <= std::numeric_limits<int16_t>::max()) {
        return ssi16(static_cast<int16_t>(value));
    } else {
        throw "integral constant too large"; // trigger compile-time error
    }
}


CONSTEVAL
ssi32 operator""_ssi32(unsigned long long value) {
    if (value <= std::numeric_limits<int32_t>::max()) {
        return ssi32(static_cast<int32_t>(value));
    } else {
        throw "integral constant too large"; // trigger compile-time error
    }
}


CONSTEVAL
ssi64 operator""_ssi64(unsigned long long value) {
    if (value <= std::numeric_limits<int64_t>::max()) {
        return ssi64(static_cast<int64_t>(value));
    } else {
        throw "integral constant too large"; // trigger compile-time error
    }
}
} // NS literals

// the following are not really needed for class types,
// because of the availability of constructors, kept for symmetry

#ifdef __cpp_concepts
template<sized_integer T>
#else
template<typename T, typename=std::enable_if_t<detail_::is_known_integer_v<T>>>
#endif
[[nodiscard]]
constexpr auto
from_int(T value) noexcept {
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
    return static_cast<result_t>(value); // no need to check, result_t corresponds to input T's range
}

#ifndef __cpp_concepts
namespace detail_{
template<typename type, typename=std::enable_if_t<is_saturatingint_v<type>,void> >
  struct numeric_limits
  {
    using ult = ULT<type>;
    static constexpr bool is_specialized = true;

    static constexpr type
    min() noexcept { return type{std::numeric_limits<ult>::min()}; }

    static constexpr type
    max() noexcept { return type{std::numeric_limits<ult>::max()}; }

    static constexpr type
    lowest() noexcept { return type{std::numeric_limits<ult>::lowest()}; }

    static constexpr int digits = std::numeric_limits<ult>::digits;
    static constexpr int digits10 = std::numeric_limits<ult>::digits10;
    static constexpr int max_digits10 = std::numeric_limits<ult>::max_digits10;
    static constexpr bool is_signed = std::numeric_limits<ult>::is_signed;
    static constexpr bool is_integer = std::numeric_limits<ult>::is_integer;
    static constexpr bool is_exact = std::numeric_limits<ult>::is_exact;
    static constexpr int radix = std::numeric_limits<ult>::radix;

    static constexpr type
    epsilon() noexcept {  return type{std::numeric_limits<ult>::epsilon()}; }

    static constexpr type
    round_error() noexcept {  return type{std::numeric_limits<ult>::round_error()}; }

    static constexpr int min_exponent = std::numeric_limits<ult>::min_exponent;
    static constexpr int min_exponent10 = std::numeric_limits<ult>::min_exponent10;
    static constexpr int max_exponent = std::numeric_limits<ult>::max_exponent;
    static constexpr int max_exponent10 = std::numeric_limits<ult>::max_exponent10;

    static constexpr bool has_infinity = std::numeric_limits<ult>::has_infinity;
    static constexpr bool has_quiet_NaN = std::numeric_limits<ult>::has_quiet_NaN;
    static constexpr bool has_signaling_NaN = std::numeric_limits<ult>::has_signaling_NaN;
    static constexpr std::float_denorm_style has_denorm
     = std::numeric_limits<ult>::has_denorm;
    static constexpr bool has_denorm_loss = std::numeric_limits<ult>::has_denorm_loss;

    static constexpr type
    infinity() noexcept { return type{std::numeric_limits<ult>::infinity()}; }

    static constexpr type
    quiet_NaN() noexcept { return type{std::numeric_limits<ult>::quiet_NaN()}; }

    static constexpr type
    signaling_NaN() noexcept
    { return type{std::numeric_limits<ult>::signaling_NaN()}; }

    static constexpr type
    denorm_min() noexcept
    { return type{std::numeric_limits<ult>::denorm_min()}; }


    static constexpr bool is_iec559 =  std::numeric_limits<ult>::is_iec559;
    static constexpr bool is_bounded =  std::numeric_limits<ult>::is_bounded;
    static constexpr bool is_modulo =  false;

    static constexpr bool traps = false;
    static constexpr bool tinyness_before =  std::numeric_limits<ult>::tinyness_before;
    static constexpr std::float_round_style round_style =  std::numeric_limits<ult>::round_style;
  };


}
#endif

} // NS satins


// provide std::numeric_limits
namespace std {
#ifdef __cpp_concepts
template<satins::a_saturatingint type>
  struct numeric_limits<type>
  {
    using ult = satins::ULT<type>;
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
    static constexpr bool is_modulo =  false; // saturation arithmetic never modulo

    static constexpr bool traps = false; // saturation arithmetic never traps
    static constexpr bool tinyness_before =  numeric_limits<ult>::tinyness_before;
    static constexpr float_round_style round_style =  numeric_limits<ult>::round_style;
  };
#else
template<>
struct numeric_limits<satins::ssi8>: satins::detail_::numeric_limits<satins::ssi8>{};
template<>
struct numeric_limits<satins::ssi16>: satins::detail_::numeric_limits<satins::ssi16>{};
template<>
struct numeric_limits<satins::ssi32>: satins::detail_::numeric_limits<satins::ssi32>{};
template<>
struct numeric_limits<satins::ssi64>: satins::detail_::numeric_limits<satins::ssi64>{};
template<>
struct numeric_limits<satins::sui8>: satins::detail_::numeric_limits<satins::sui8>{};
template<>
struct numeric_limits<satins::sui16>: satins::detail_::numeric_limits<satins::sui16>{};
template<>
struct numeric_limits<satins::sui32>: satins::detail_::numeric_limits<satins::sui32>{};
template<>
struct numeric_limits<satins::sui64>: satins::detail_::numeric_limits<satins::sui64>{};

#endif
}

#endif /* SRC_PSSSATIN_ */
