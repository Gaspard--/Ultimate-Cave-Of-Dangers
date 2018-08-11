#pragma once

#include "FixedPoint.hpp"
#include "Vect.hpp"
#include "Display.hpp"

namespace disp
{
  enum class TextureList;
}

namespace logic
{
  class Logic;

  class Entity
  {
    Vect<FixedPoint<-8>, 2u> position;
    Vect<FixedPoint<-8>, 2u> speed;
    unsigned int grounded;
    Vect<unsigned int, 2u> onWall;
    disp::TextureList texture;
  public:
    Entity();

    void update(Logic &);
    void jump() noexcept;
    bool shouldBeRemoved() const noexcept;
    disp::TextureList const & getTexture() const;
    Vect<FixedPoint<-8>, 2u> const & getPosition() const;
  };
};
