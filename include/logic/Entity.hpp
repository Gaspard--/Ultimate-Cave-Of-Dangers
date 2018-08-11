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
  public:
    Entity();

    void update(Logic &);
    bool shouldBeRemoved() const;
  };
};
