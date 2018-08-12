#pragma once

namespace disp
{
  struct Camera
  {
    Vect<float, 2u> offset{0.0f, 0.0f};
    Vect<float, 2u> zoom{1.0f, 1.0f};

    template<class T>
    constexpr Vect<float, 2u> apply(Vect<T, 2u> pos) const
    {
      return Vect<float, 2u>(pos - offset) * zoom;
    }

    template<class T>
    constexpr Vect<double, 2u> unapply(Vect<T, 2u> pos) const
    {
      return  static_cast<Vect<float, 2u>>(pos / zoom) - offset;
    }
  };
}
