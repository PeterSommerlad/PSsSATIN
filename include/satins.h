#ifndef PS_SATINS_H_
#define PS_SATINS_H_

#include <cstdint>
#include <type_traits>
#include <iosfwd>
#include <limits>
#include <climits>
#include <compare>
#include <concepts>
#if __cplusplus > 202302L
#include <numeric> // C++26 saturation arithmetic
#endif

namespace satins { // SATuration Integral Numbers with struct
namespace detail_ {
template<typename T>
using plain = std::remove_cvref_t<T>;

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

// only support the following sizes:
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
template<typename TESTED>
concept sized_integer = detail_::is_known_integer_v<TESTED>;
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
template<sized_integer T>
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
template<sized_integer T>
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
template<sized_integer T>
[[nodiscard]]
constexpr bool non_builtin_mul_overflow(T l, T r, T* result) noexcept {
    if constexpr (std::numeric_limits<T>::is_signed){
        if constexpr(sizeof(T) == sizeof(std::int64_t)){
            bool resultnegative { (l < 0) != (r < 0) };
            uint64_t res{};
            auto abs64 { [](int64_t value) -> uint64_t { return value < 0? 1ULL + ~static_cast<uint64_t>(value):static_cast<uint64_t>(value);} };
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
template<sized_integer T>
[[nodiscard]]
constexpr bool add_overflow(T l, T r, T* result) noexcept {
    return __builtin_add_overflow(l,r,result);
}
template<sized_integer T>
[[nodiscard]]
constexpr bool sub_overflow(T l, T r, T* result) noexcept {
    return __builtin_sub_overflow(l,r,result);
}
template<sized_integer T>
[[nodiscard]]
constexpr bool mul_overflow(T l, T r, T* result) noexcept {
    return __builtin_mul_overflow(l,r,result);
}

#else // DIY
template<sized_integer T>
[[nodiscard]]
constexpr bool add_overflow(T l, T r, T* result) noexcept {
    return non_builtin::non_builtin_add_overflow(l,r,result);
}
template<sized_integer T>
[[nodiscard]]
constexpr bool sub_overflow(T l, T r, T* result) noexcept {
    return non_builtin::non_builtin_sub_overflow(l,r,result);
}
template<sized_integer T>
[[nodiscard]]
constexpr bool mul_overflow(T l, T r, T* result) noexcept {
    return non_builtin::non_builtin_mul_overflow(l,r,result);
}

#endif

#endif



} // NS detail_


template<sized_integer INT>
struct Satin;

namespace detail_{
template<typename T>
constexpr bool is_saturatingint_v = false;
template<sized_integer I>
constexpr bool is_saturatingint_v<Satin<I>> = true;

template<typename C>
struct ULT_impl;
template<typename I>
struct ULT_impl<Satin<I>>{
    using type=I;
};
} // NS detail_
template<typename E>
concept a_saturatingint = detail_::is_saturatingint_v<E>;

template<typename C>
using ULT=detail_::ULT_impl<detail_::plain<C>>::type;

namespace detail_ {

template<typename E>
using promoted_t = // will promote the underlying type keeping signedness
        std::conditional_t<(sizeof(ULT<E>) < sizeof(int))
            , std::conditional_t<std::is_unsigned_v<ULT<E>>
                , unsigned
                , int >
            , ULT<E>>;

template<a_saturatingint E>
[[nodiscard]]
constexpr auto
promote_keep_signedness(E value) noexcept
{ // promote keeping signedness
    return static_cast<promoted_t<E>>(static_cast<ULT<E>>(value));// promote with sign extension
}


template<a_saturatingint E>
[[nodiscard]]
constexpr auto
promote_to_unsigned(E value) noexcept
{ // promote to unsigned for wrap around arithmetic
    using u_result_t = std::make_unsigned_t<promoted_t<E>>;
    return static_cast<u_result_t>(promote_keep_signedness(value));
}
template<sized_integer TARGET, a_saturatingint E>
[[nodiscard]]
constexpr auto
promote_and_extend_to_unsigned(E value) noexcept
{ // promote to unsigned for wrap around arithmetic, with sign extension if needed
       using u_result_t = std::conditional_t< (sizeof(TARGET) > sizeof(promoted_t<E>)),
                std::make_unsigned_t<TARGET>, std::make_unsigned_t<promoted_t<E> > >;
       using s_result_t = std::make_signed_t<u_result_t>;
       return static_cast<u_result_t>(static_cast<s_result_t>(promote_keep_signedness(value)));// promote with sign extension
}


} // NS detail_

template<typename LEFT, typename RIGHT>
constexpr bool
same_signedness_v = std::numeric_limits<LEFT>::is_signed == std::numeric_limits<RIGHT>::is_signed;

template<typename LEFT, typename RIGHT>
concept same_signedness = same_signedness_v<LEFT,RIGHT>;

// path tests are compile-time checked:
template<a_saturatingint result_t, sized_integer FROM>
[[nodiscard]]
constexpr auto
from_int_to(FROM value) noexcept
{
    using ultr = ULT<result_t>;
#ifdef __cpp_lib_saturation_arithmetic
    return static_cast<result_t>(std::saturate_cast<ultr>(value));
#else
   if constexpr(not std::is_same_v<ultr,FROM>) {
        if constexpr (std::is_unsigned_v<ultr>){
            if constexpr (std::is_signed_v<FROM>){
                if (value < FROM{}){
                    return result_t{}; // zero
                }// value is positive below
            }
            if (static_cast<uint64_t>(value) > std::numeric_limits<ultr>::max()) {
                return std::numeric_limits<result_t>::max();
            }
        } else { // to signed
            if constexpr (std::is_signed_v<FROM>) {
                if (static_cast<int64_t>(value) < std::numeric_limits<ultr>::min()) {
                    return std::numeric_limits<result_t>::min();
                }
            }
            if (static_cast<uint64_t>(value) > std::numeric_limits<ultr>::max()) {
                return std::numeric_limits<result_t>::max();
            }
        }
    }
    return static_cast<result_t>(static_cast<ultr>(value)); // value is checked above
#endif
}


template<sized_integer INT>
struct Satin{
    constexpr Satin() noexcept:value_which_should_not_be_referred_to_from_user_code{}{}
    explicit constexpr Satin(std::same_as<INT> auto v) noexcept:value_which_should_not_be_referred_to_from_user_code(v){
    }
    friend constexpr auto operator<=>(Satin, Satin) noexcept = default;
    explicit constexpr operator INT() const noexcept { return value_which_should_not_be_referred_to_from_user_code;}
    template<sized_integer T>
    explicit constexpr Satin(T v)
    requires (not std::same_as<INT,detail_::plain<T>>)
    :value_which_should_not_be_referred_to_from_user_code(from_int_to<Satin>(v)){}

    // member/friend operators

    // negation for signed types only, two's complement
    constexpr auto
    operator-() const noexcept
    requires std::numeric_limits<INT>::is_signed
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

    template<a_saturatingint RIGHT>
    friend constexpr auto
    operator+(Satin l, RIGHT r) noexcept
    requires same_signedness<Satin,RIGHT>
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


    template<a_saturatingint RIGHT>
    constexpr auto&
    operator+=(RIGHT r)  & noexcept
    requires same_signedness<Satin,RIGHT>
    {
        static_assert(sizeof(Satin) >= sizeof(RIGHT),"satins: adding too large integer type");
        *this = static_cast<Satin>(*this+r);
        return *this;
    }

    template<a_saturatingint RIGHT>
    friend constexpr auto
    operator-(Satin l, RIGHT r) noexcept
    requires same_signedness<Satin,RIGHT>
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
    template<a_saturatingint RIGHT>
    constexpr auto&
    operator-=(RIGHT r) & noexcept
    requires same_signedness<Satin,RIGHT>
    {
        static_assert(sizeof(Satin) >= sizeof(RIGHT),"subtracting too large integer type");
        *this = static_cast<Satin>(*this-r);
        return *this;
    }


    template<a_saturatingint RIGHT>
    friend constexpr auto
    operator*(Satin l, RIGHT r) noexcept
    requires same_signedness<Satin,RIGHT>
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
    template<a_saturatingint RIGHT>
    constexpr auto&
    operator*=(RIGHT r) & noexcept
    requires same_signedness<Satin,RIGHT>
    {
        static_assert(sizeof(Satin) >= sizeof(RIGHT),"multiplying too large integer type");
        *this = static_cast<Satin>(*this*r);
        return *this;
    }
    template<a_saturatingint RIGHT>
    friend constexpr auto
    operator/(Satin const l, RIGHT const r) noexcept
    requires same_signedness<Satin,RIGHT>
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
    template<a_saturatingint RIGHT>
    constexpr auto&
    operator/=(RIGHT r) & noexcept
    requires same_signedness<Satin,RIGHT>
    {
        static_assert(sizeof(Satin) >= sizeof(RIGHT),"dividing by too large integer type");
        *this = static_cast<Satin>(*this/r);
        return *this;
    }
    template<a_saturatingint RIGHT>
    friend constexpr auto
    operator%(Satin l, RIGHT r) noexcept
    requires std::is_unsigned_v<ULT<Satin>> && std::is_unsigned_v<ULT<RIGHT>>
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
    template<a_saturatingint RIGHT>
    constexpr auto&
    operator%=(RIGHT r) & noexcept
    requires std::is_unsigned_v<ULT<Satin>> && std::is_unsigned_v<ULT<RIGHT>>
    {
        static_assert(sizeof(Satin) >= sizeof(RIGHT),"dividing by too large integer type");
        *this = static_cast<Satin>(*this % r);
        return *this;
    }
    // bitwise operators

    template<a_saturatingint RIGHT>
    friend constexpr auto
    operator&(Satin l, RIGHT r) noexcept
    requires std::is_unsigned_v<ULT<Satin>> && std::is_unsigned_v<ULT<RIGHT>>
    {
        using detail_::promote_keep_signedness;

        using result_t=std::conditional_t<sizeof(Satin)>=sizeof(RIGHT),Satin,RIGHT>;
        return static_cast<result_t>(promote_keep_signedness(l)&promote_keep_signedness(r));
    }
    template<a_saturatingint RIGHT>
    constexpr auto&
    operator&=(RIGHT r) & noexcept
    requires std::is_unsigned_v<ULT<Satin>> && std::is_unsigned_v<ULT<RIGHT>>
    {
        static_assert(sizeof(Satin) == sizeof(RIGHT),"bitand by different sized integer type");
        *this = static_cast<Satin>(*this&r);
        return *this;
    }

    template<a_saturatingint RIGHT>
    friend constexpr auto
    operator|(Satin l, RIGHT r) noexcept
    requires std::is_unsigned_v<ULT<Satin>> && std::is_unsigned_v<ULT<RIGHT>>
    {
        using detail_::promote_keep_signedness;
        using result_t=std::conditional_t<sizeof(Satin)>=sizeof(RIGHT),Satin,RIGHT>;
        return static_cast<result_t>(promote_keep_signedness(l)|promote_keep_signedness(r));
    }
    template<a_saturatingint RIGHT>
    constexpr auto&
    operator|=(RIGHT r) & noexcept
    requires std::is_unsigned_v<ULT<Satin>> && std::is_unsigned_v<ULT<RIGHT>>
    {
        static_assert(sizeof(Satin) == sizeof(RIGHT),"bitor by different sized integer type");
        *this = static_cast<Satin>(*this|r);
        return *this;
    }

    template<a_saturatingint RIGHT>
    friend constexpr auto
    operator^(Satin l, RIGHT r) noexcept
    requires std::is_unsigned_v<ULT<Satin>> && std::is_unsigned_v<ULT<RIGHT>>
    {
        using detail_::promote_keep_signedness;

        using result_t=std::conditional_t<sizeof(Satin)>=sizeof(RIGHT),Satin,RIGHT>;
        return static_cast<result_t>(promote_keep_signedness(l)^promote_keep_signedness(r));
    }
    template<a_saturatingint RIGHT>
    constexpr auto&
    operator^=(RIGHT r) & noexcept
    requires std::is_unsigned_v<ULT<Satin>> && std::is_unsigned_v<ULT<RIGHT>>
    {
        static_assert(sizeof(Satin) == sizeof(RIGHT),"xor by different sized integer type");
        *this = static_cast<Satin>(*this^r);
        return *this;
    }


    friend constexpr Satin
    operator~(Satin l) noexcept
    requires std::is_unsigned_v<ULT<Satin>>
    {
        return static_cast<Satin>(static_cast<ULT<Satin>>(~detail_::promote_keep_signedness(l)));
    }


    template<a_saturatingint RIGHT>
    friend constexpr Satin
    operator<<(Satin l, RIGHT r) noexcept
    requires std::is_unsigned_v<ULT<Satin>> && std::is_unsigned_v<ULT<RIGHT>>
    {
        using detail_::promote_keep_signedness;
        if( static_cast<size_t>(promote_keep_signedness(r)) < sizeof(Satin)*CHAR_BIT){
            return static_cast<Satin>(promote_keep_signedness(l)<<promote_keep_signedness(r));
        } else {
            return Satin{}; // zero
        }
    }
    template<a_saturatingint RIGHT>
    constexpr auto&
    operator<<=(RIGHT r) & noexcept
    requires std::is_unsigned_v<ULT<Satin>> && std::is_unsigned_v<ULT<RIGHT>>
    {
        *this = (*this<<r);
        return *this;
    }
    template<a_saturatingint RIGHT>
    friend constexpr Satin
    operator>>(Satin l, RIGHT r) noexcept
    requires std::is_unsigned_v<ULT<Satin>> && std::is_unsigned_v<ULT<RIGHT>>
    {   using detail_::promote_keep_signedness;
        if( static_cast<size_t>(promote_keep_signedness(r)) < sizeof(Satin)*CHAR_BIT){
            return static_cast<Satin>(
                    promote_keep_signedness(l)>>promote_keep_signedness(r));
        } else {
            return Satin{}; // zero
        }
    }
    template<a_saturatingint RIGHT>
    constexpr auto&
    operator>>=( RIGHT r) & noexcept
    requires std::is_unsigned_v<ULT<Satin>> && std::is_unsigned_v<ULT<RIGHT>>
    {
        *this = (*this >>r);
        return *this;
    }


    friend std::ostream& operator<<(std::ostream &out, Satin value){
        out << detail_::promote_keep_signedness(value);
        return out;
    }
    // no need for private, makes compilability checks possible
    // all possible values of INT are valid
    INT value_which_should_not_be_referred_to_from_user_code;
};
// unsigned

using sui8  = Satin<std::uint8_t >;
using sui16 = Satin<std::uint16_t>;
using sui32 = Satin<std::uint32_t>;
using sui64 = Satin<std::uint64_t>;

inline namespace literals {
consteval
sui8 operator""_sui8(unsigned long long value) {
    if (value <= std::numeric_limits<std::uint8_t>::max()) {
        return sui8(static_cast<std::uint8_t>(value));
    } else {
        throw "integral constant too large"; // trigger compile-time error
    }
}


consteval
sui16 operator""_sui16(unsigned long long value) {
    if (value <= std::numeric_limits<std::uint16_t>::max()) {
        return sui16(static_cast<std::uint16_t>(value));
    } else {
        throw "integral constant too large"; // trigger compile-time error
    }
}


consteval
sui32 operator""_sui32(unsigned long long value) {
    if (value <= std::numeric_limits<uint32_t>::max()) {
        return sui32(static_cast<std::uint32_t>(value));
    } else {
        throw "integral constant too large"; // trigger compile-time error
    }
}


consteval
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
consteval
ssi8 operator""_ssi8(unsigned long long value) {
    if (value <= std::numeric_limits<std::int8_t>::max()) {
        return ssi8(static_cast<int8_t>(value));
    } else {
        throw "integral constant too large"; // trigger compile-time error
    }
}


consteval
ssi16 operator""_ssi16(unsigned long long value) {
    if (value <= std::numeric_limits<int16_t>::max()) {
        return ssi16(static_cast<int16_t>(value));
    } else {
        throw "integral constant too large"; // trigger compile-time error
    }
}


consteval
ssi32 operator""_ssi32(unsigned long long value) {
    if (value <= std::numeric_limits<int32_t>::max()) {
        return ssi32(static_cast<int32_t>(value));
    } else {
        throw "integral constant too large"; // trigger compile-time error
    }
}


consteval
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

template<sized_integer T>
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

} // NS satins


// provide std::numeric_limits
namespace std {

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

}

#endif /* SRC_PSSSATIN_ */
