#pragma once

template<int exponent>
class FixedPoint
{
  using StoreType = int;
  using UnsignedType = unsigned int;
  StoreType value;

public:
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
  : value(UnsignedType(other.value) << (otherExponent - exponent))
  {
    static_assert(otherExponent - exponent < 16 && exponent - otherExponent < 16); // sanity to not lose to much precision
  }

#define FIXED_POINT_OPEARTOR(OP)					\
  constexpr FixedPoint &operator OP##=(FixedPoint const &other) noexcept \
  {									\
    value OP other.value;						\
    return *this;							\
  }									\
    									\
  template<int otherExponent>						\
  constexpr FixedPoint<exponent + otherExponent> operator OP(FixedPoint<otherExponent> const &other) const noexcept \
  {									\
    return FixedPoint<exponent + otherExponent>(value * other.value);	\
  }									\

  FIXED_POINT_OPEARTOR(+);
  FIXED_POINT_OPEARTOR(-);
  FIXED_POINT_OPEARTOR(*);
  FIXED_POINT_OPEARTOR(/);
  FIXED_POINT_OPEARTOR(%);

  constexpr float getFloatValue() const noexcept
  {
    return float(StoreType(UnsignedType(value) << exponent));
  }
};
