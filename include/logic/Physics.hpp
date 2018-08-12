#pragma once

#include "Logic.hpp"

#include <algorithm>

namespace logic
{
  static constexpr FixedPoint<-8> WaterDropSize{256};

  inline void resolve(WaterDrop &a, WaterDrop &b) noexcept
  {
    auto distance(a.position - b.position);
    auto length2(distance.length2());

    if (!!length2 && length2 < WaterDropSize * WaterDropSize)
      {
	a.speed += distance / length2 * FixedPoint<-16>{1};
	b.speed -= distance / length2 * FixedPoint<-16>{1};
      }
  }

  template<class It>
  void resolve(It begin, It end) noexcept
  {
    for (auto it1(begin); it1 < end; ++it1)
      for (auto it2(it1 + 1); it2 < end; ++it2)
	resolve(*it1, *it2);
  }

  template<bool dir = 0, class It>
  void checkCollisions(It begin, It end) noexcept
  {
    if (end - begin < 10)
      resolve(begin, end);
    uint32_t initial(begin->position[dir].value);
    uint32_t mask(0);
    std::for_each(begin, end, [initial, &mask](WaterDrop const &drop)
		  {
		    mask |= (drop.position[dir] - WaterDropSize).value ^ initial;
		    mask |= (drop.position[dir] + WaterDropSize).value ^ initial;
		  });
    uint32_t i(0u);
    while (mask >> i);
    mask = (1u << i) - 1;
    checkCollisions<!dir>(begin, std::partition(begin, end, [mask, i](WaterDrop const &drop)
						{
						  return ((drop.position[dir] + WaterDropSize).value & mask) > (1u << i - 1u);
						}));
    checkCollisions<!dir>(std::partition(begin, end, [mask, i](WaterDrop const &drop)
					 {
					   return ((drop.position[dir] - WaterDropSize).value & mask) > (1u << i - 1u);
					 }), end);
  }
};
