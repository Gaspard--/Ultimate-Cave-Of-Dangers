#pragma once

#include <type_traits>
#include <cmath>

template<int exponent, class StoreType = unsigned int>
class FixedPoint
{
  template<int shift, class Type>
  static constexpr Type logicalLShift(Type val) noexcept
  {
    if constexpr (std::is_signed_v<Type>)
      {
	if (val > 0)
	  return logicalLShift<shift>(std::make_unsigned_t<Type>(val));
	else
	  return -logicalLShift<shift>(std::make_unsigned_t<Type>(-val));
      }
    else
      {
	if constexpr (shift < 0)
          return val >> -shift;
	else
	  return val << shift;
      }
  }
  
public:
  StoreType value;

  static constexpr const FixedPoint One{logicalLShift<-exponent>(1u)};
  static constexpr const FixedPoint Zero{0};

  constexpr FixedPoint() noexcept
  : value(0)
  {
  }

  explicit constexpr FixedPoint(StoreType value) noexcept
  : value(value)
  {
  }

  template<int otherExponent, class OtherStoreType>
  constexpr FixedPoint(FixedPoint<otherExponent, OtherStoreType> other) noexcept
  : value(logicalLShift<otherExponent - exponent>(other.value))
  {
    static_assert(otherExponent - exponent <= 16 && exponent - otherExponent <= 16); // sanity to not lose to much precision
  }

  // constexpr FixedPoint &operator*=(FixedPoint<0, StoreType> const &other) noexcept
  // {
  //   value *= other.value;
  //   return *this;
  // }

  template<int otherExponent>
  constexpr FixedPoint<exponent + otherExponent, StoreType> operator*(FixedPoint<otherExponent, StoreType> const &other) const noexcept
  {	
    return FixedPoint<exponent + otherExponent, StoreType>(value * other.value);
  }

  template<int otherExponent>
  constexpr FixedPoint &operator*=(FixedPoint<otherExponent, StoreType> const &other) noexcept
  {
    return *this = FixedPoint(*this * other);
  }

  template<int otherExponent, class OtherType>
  constexpr FixedPoint &operator/=(FixedPoint<0, OtherType> const &other) noexcept
  {
    value /= other.value;
    return *this;
  }

  template<int otherExponent, class OtherType>
  constexpr FixedPoint<exponent - otherExponent, StoreType> operator/(FixedPoint<otherExponent, OtherType> const &other) const noexcept
  {
    return FixedPoint<exponent - otherExponent>(value / other.value);
  }

  template<int otherExponent, class OtherType>
  constexpr FixedPoint &operator/=(FixedPoint<otherExponent, OtherType> const &other) noexcept
  {
    return *this = FixedPoint(*this / other);
  }

  template<int otherExponent, class OtherType>
  constexpr FixedPoint &operator-=(FixedPoint<otherExponent, OtherType> const &other) noexcept
  {
    value -= FixedPoint<exponent, OtherType>(other).value;
    return *this;
  }

  template<class OtherType>
  constexpr FixedPoint operator-(FixedPoint<exponent, OtherType> const &other) const noexcept
  {
    return FixedPoint(value - other.value);
  }

  template<int otherExponent, class OtherType>
  constexpr FixedPoint &operator+=(FixedPoint<otherExponent, OtherType> const &other) noexcept
  {
    value += FixedPoint<exponent, OtherType>(other).value;
    return *this;
  }

  template<class OtherType>
  constexpr FixedPoint operator+(FixedPoint<exponent, OtherType> const &other) const noexcept
  {
    return FixedPoint(value + other.value);
  }

  template<class OtherType>
  constexpr FixedPoint operator%(FixedPoint<exponent, OtherType> const &other) const noexcept
  {
    return FixedPoint(value % other.value);
  }

  constexpr FixedPoint operator-() const noexcept
  {
    return FixedPoint(-value);
  }

  constexpr FixedPoint floor() const noexcept
  {
    return FixedPoint<0, StoreType>(*this);
  }

  constexpr bool isPositive() const noexcept
  {
    return value > 0;
  }

  constexpr bool isNegative() const noexcept
  {
    return value < 0;
  }

  constexpr bool operator!() const noexcept
  {
    return !value;
  }

#define FIXEDPOINT_RELTATIVE_OP(OP)					\
  template<class OtherType>						\
  constexpr bool operator OP (FixedPoint<exponent, OtherType> const &other) const noexcept \
  {									\
    return value OP other.value;					\
  }

  FIXEDPOINT_RELTATIVE_OP(==);
  FIXEDPOINT_RELTATIVE_OP(!=);
  FIXEDPOINT_RELTATIVE_OP(<=);
  FIXEDPOINT_RELTATIVE_OP(>=);
  FIXEDPOINT_RELTATIVE_OP(>);
  FIXEDPOINT_RELTATIVE_OP(<);

  constexpr float getFloatValue() const noexcept
  {
    return float(std::ldexp(float(value), exponent));
  }
};
