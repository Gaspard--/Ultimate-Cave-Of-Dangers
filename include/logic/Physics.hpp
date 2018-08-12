#pragma once

#include "Logic.hpp"

#include <algorithm>

namespace logic
{
  inline void resolve(WaterDrop &a, WaterDrop &b) noexcept
  {
    auto distance(Vect<FixedPoint<-8, int>, 2u>(a.position) - Vect<FixedPoint<-8, int>, 2u>(b.position));
    auto length2(distance.length2());

    if (!!length2 && length2 < WaterDropSize * WaterDropSize)
      {
	length2 = std::min(length2, FixedPoint<-16, int>(WaterDropSize * WaterDropSize * FixedPoint<-2>(1)));
	auto scale(FixedPoint<-8, int>(1) * FixedPoint<-8, int>::One);
	
	Vect<decltype(scale * (distance[0]) / length2), 2u> diff(scale * distance[0] / length2,
								 scale * distance[1] / length2);

	a.speed += diff;
	b.speed -= diff;
      }
  }

  template<class It>
  void resolve(It begin, It end) noexcept
  {
    for (auto it1(begin); it1 < end; ++it1)
      for (auto it2(it1 + 1); it2 < end; ++it2)
	resolve(*it1, *it2);
  }

  template<bool dir = 0, bool otherDir = 1, class It>
  void checkCollisions(It begin, It end, Vect<size_t, 2u> depth = {0, 0}) noexcept
  {
    if (std::distance(begin, end) < 10)
      return resolve(begin, end);
    uint32_t initial(begin->position[dir].value);
    uint32_t mask(0);
    std::for_each(begin, end, [initial, &mask](WaterDrop const &drop)
		  {
		    mask |= (drop.position[dir] - WaterDropSize).value ^ initial;
		    mask |= (drop.position[dir] + WaterDropSize).value ^ initial;
		  });
    uint32_t i(0u);
    while (i != 32 && mask >> i)
      ++i;
    if (i == 32)
      return resolve(begin, end);
    if (i == depth[dir])
      {
	if constexpr (dir == otherDir)
          return resolve(begin, end);
	else
	  return checkCollisions<otherDir, otherDir>(begin, end, depth);
      }
    mask = (1u << i) - 1;      
    depth[dir] = i;
    checkCollisions<otherDir, dir>(begin, std::partition(begin, end, [mask, i](WaterDrop const &drop)
							 {
							   return ((drop.position[dir] + WaterDropSize).value & mask) > (1u << i - 1u);
							 }), depth);
    checkCollisions<otherDir, dir>(std::partition(begin, end, [mask, i](WaterDrop const &drop)
						  {
						    return ((drop.position[dir] - WaterDropSize).value & mask) > (1u << i - 1u);
						  }), end, depth);
  }
};
