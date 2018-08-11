#pragma once

#include <type_traits>

template<int exponent, class StoreType = unsigned int>
class FixedPoint
{
  template<int shift, class Type>
  static constexpr Type logicalLShift(Type val) noexcept
  {
    if constexpr (std::is_signed_v<Type>)
      {
	if (val > 0)
	  return logicalLShift(std::make_unsigned_t<Type>(val));
	else
	  return -logicalLShift(std::make_unsigned_t<Type>(-val));
      }
    else
      {
	if constexpr (shift < 0)
          return static_cast<unsigned int>(val) >> -shift;
	else
	  return static_cast<unsigned int>(val) << shift;
      }
  }
  
public:
  StoreType value;

  constexpr FixedPoint() noexcept
  : value(0)
  {
  }

  explicit constexpr FixedPoint(StoreType value) noexcept
  : value(value)
  {
  }

  template<int otherExponent>
  explicit constexpr FixedPoint(FixedPoint<otherExponent> other) noexcept
  : value(logicalLShift<otherExponent - exponent>(other.value))
  {
    static_assert(otherExponent - exponent < 16 && exponent - otherExponent < 16); // sanity to not lose to much precision
  }

  constexpr FixedPoint &operator*=(FixedPoint const &other) noexcept
  {									
    value *= other.value;						
    return *this;							
  }

  template<int otherExponent>
  constexpr FixedPoint<exponent + otherExponent> operator*(FixedPoint<otherExponent, StoreType> const &other) const noexcept
  {	
    return FixedPoint<exponent + otherExponent>(value * other.value);
  }

  template<int otherExponent>
  constexpr FixedPoint &operator*=(FixedPoint<otherExponent, StoreType> const &other) noexcept
  {				
    return *this = FixedPoint(other * *this);
  }

  constexpr FixedPoint &operator/=(FixedPoint<0, StoreType> const &other) noexcept
  {									
    value /= other.value;						
    return *this;							
  }

  template<int otherExponent>
  constexpr FixedPoint<exponent - otherExponent> operator/(FixedPoint<otherExponent, StoreType> const &other) const noexcept
  {	
    return FixedPoint<exponent - otherExponent>(value / other.value);
  }

  template<int otherExponent>
  constexpr FixedPoint &operator/=(FixedPoint<otherExponent, StoreType> const &other) noexcept
  {				
    return *this = FixedPoint(other / *this);
  }

  template<int otherExponent, class OtherType>
  constexpr FixedPoint &operator-=(FixedPoint<otherExponent, OtherType> const &other) noexcept
  {
    value -= FixedPoint<exponent, OtherType>(other).value;
    return *this;
  }

  template<int otherExponent, class OtherType>
  constexpr FixedPoint &operator+=(FixedPoint<otherExponent, OtherType> const &other) noexcept
  {
    value += FixedPoint<exponent, OtherType>(other).value;
    return *this;
  }
  
  constexpr float getFloatValue() const noexcept
  {
    return float(logicalLShift<exponent>(value));
  }
};
