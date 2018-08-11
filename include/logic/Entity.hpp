#pragma once

#include "FixedPoint.hpp"
#include "Vect.hpp"

namespace logic
{
  class Logic;

  class Entity
  {
    Vect<FixedPoint<-8>, 2u> position;
    Vect<FixedPoint<-8>, 2u> speed;
    unsigned int grounded;
    Vect<unsigned int, 2u> onWall;
  public:
    Entity();

    void update(Logic &);
    void jump() noexcept;
    bool shouldBeRemoved() const noexcept;
  };
};
