#pragma once

#include <type_traits>
#include <utility>
#include <math.h>
#include <cstddef>
# include "FixedPoint.hpp"

// taken from raven-os/claws, will reintegrate major improvements
template<class T, std::size_t dim>
class Vect
{
private:
  T data[dim];

  template<class V, std::size_t... indices>
  constexpr Vect(V const &other, std::index_sequence<indices...>)
  : data{static_cast<T>(other[indices])...}
  {}

  template<std::size_t... indices>
  constexpr Vect(std::index_sequence<indices...>)
    : Vect(((void)indices, T{})...)
  {}

  template<class func_type, std::size_t... indices>
  static auto apply_op(func_type &&func, std::index_sequence<indices...>)
  {
    return Vect{func(indices)...};
  }

public:
  template<class func_type>
  static Vect<T, dim> apply_op(func_type &&func)
  {
    return apply_op(func, std::make_index_sequence<dim>{});
  }

  template<class V>
  constexpr Vect(V const (&other)[dim])
    : Vect(other, std::make_index_sequence<dim>{})
  {}

  template<class V>
  constexpr Vect(Vect<V, dim> const &other)
  : Vect(other, std::make_index_sequence<dim>{})
  {}

  constexpr Vect()
  : Vect(std::make_index_sequence<dim>{})
  {}

  template<class... U, typename std::enable_if<sizeof...(U) == dim>::type * = nullptr>
  constexpr Vect(U &&... ts)
  : data{std::forward<U>(ts)...}
  {}

  constexpr T *begin()
  {
    return data;
  }

  constexpr T *end()
  {
    return data + dim;
  }

  constexpr T const *begin() const
  {
    return data;
  }

  constexpr T const *end() const
  {
    return data + dim;
  }

  template<class operation>
  void apply_on_self(operation op)
  {
    for (unsigned int i(0); i < dim; ++i)
      data[i] = op(i);
  }

  constexpr T &operator[](std::size_t index)
  {
    return (data[index]);
  }

  constexpr T const &operator[](std::size_t index) const
  {
    return (data[index]);
  }

#define VECT_OPERATOR_DEF(OP)						\
  template<class U>							\
  constexpr Vect<T, dim> &operator OP##=(Vect<U, dim> const &other)	\
  {									\
    for (std::size_t i(0u); i != dim; ++i)				\
      data[i] OP## = other[i];						\
    return (*this);							\
  };									\
									\
  template<class U>							\
  constexpr auto operator OP(Vect<U, dim> const &other) const		\
  {									\
    Vect<decltype(data[0] OP other[0]), dim> result{*this};	        \
									\
    result OP## = other;						\
    return result;							\
  };									\
									\
  template<class U>							\
  constexpr Vect<T, dim> &operator OP##=(U const &other)		\
  {									\
    for (auto &elem : *this)						\
      elem OP## = other;						\
    return (*this);							\
  };									\
									\
  template<class U>							\
  constexpr auto operator OP(U const &other) const			\
  {									\
    Vect<decltype(data[0] OP other), dim> result{*this};		\
									\
    result OP## = other;						\
    return result;							\
  }

  VECT_OPERATOR_DEF(+);
  VECT_OPERATOR_DEF(-);
  VECT_OPERATOR_DEF(*);
  VECT_OPERATOR_DEF(/);
  VECT_OPERATOR_DEF(%);
  VECT_OPERATOR_DEF (^);
  VECT_OPERATOR_DEF(|);
  VECT_OPERATOR_DEF(&);

  // TODO optimize ?
  constexpr bool equals(Vect<T, dim> const &other) const
  {
    unsigned int i(0);

    for (; i != dim && data[i] == other.data[i]; ++i)
      ;
    return i == dim;
  }

private:
  template<class FUNC, size_t... indices>
  constexpr Vect<T, dim> map_impl(FUNC &&func, std::index_sequence<indices...>) const
  {
    return {func(data[indices])...};
  }

public:
  template<class FUNC>
  constexpr Vect<T, dim> map(FUNC &&func) const
  {
    return (map_impl(func, std::make_index_sequence<dim>{}));
  }

#define VECT_UNARY_OP_DEF(OP)				\
  constexpr Vect<T, dim> operator OP(void) const	\
  {							\
    struct mapper					\
    {							\
      T operator()(T const &t)				\
      {							\
        return OP t;					\
      }							\
    };							\
    return map(mapper{});				\
  }

  VECT_UNARY_OP_DEF(-);
  VECT_UNARY_OP_DEF(+);
  VECT_UNARY_OP_DEF(~);
  VECT_UNARY_OP_DEF(!);

public:
  constexpr T sum(void) const
  {
    T result{0u};

    for (auto const &t : *this)
      result += t;
    return (result);
  }

  constexpr T scalar(Vect<T, dim> const &other) const
  {
    return ((*this * other).sum());
  }

  constexpr auto length2() const
  {
    return ((*this * *this).sum());
  }

  Vect<T, dim> normalized() const
  {
    return length2() > 0 ? ((*this) / sqrt(length2())) : *this;
  }

#define VECT_NAMED_COMPONENT(NAME, INDEX)				\
  template<unsigned int _dim = dim, typename std::enable_if<(_dim > 0)>::type * = nullptr> \
  T NAME() const							\
  {									\
    return (data[INDEX]);						\
  }									\
									\
  template<unsigned int _dim = dim, typename std::enable_if<(_dim > 0)>::type * = nullptr> \
  T &NAME()								\
  {									\
    return (data[INDEX]);						\
  }

  VECT_NAMED_COMPONENT(x, 0);
  VECT_NAMED_COMPONENT(y, 1);
  VECT_NAMED_COMPONENT(z, 2);
  VECT_NAMED_COMPONENT(w, 3);

  constexpr bool all() const
  {
    unsigned int i(0);

    while (i != dim && data[i])
      i = i + 1;
    return (i == dim);
  }

  constexpr bool any() const
  {
    unsigned int i(0);

    while (i != dim && !data[i])
      i = i + 1;
    return (i != dim);
  }

  template<class Type>
  static Vect<float, dim> fromFixedPoint(const Vect<Type, dim>& fixedPointVect)
  {
    Vect<float, dim> floatVect;

    for (std::size_t i = 0 ; i < dim ; ++i)
      floatVect[i] = fixedPointVect[i].getFloatValue();
    return floatVect;
  }
};
