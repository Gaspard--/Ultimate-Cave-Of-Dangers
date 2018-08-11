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
    Vect<FixedPoint<-8>, 2u> size;
    Vect<FixedPoint<-8>, 2u> position;
    Vect<FixedPoint<-8, int>, 2u> speed;
    unsigned int grounded;
    Vect<unsigned int, 2u> onWall;
    disp::TextureList texture;
  public:
    Entity(disp::TextureList);

    void update(Logic &);
    void jump() noexcept;
    void drift(int dir) noexcept;
    void dash(int dir) noexcept;
    bool shouldBeRemoved() const noexcept;

    disp::TextureList const & getTexture() const noexcept
    {
      return (texture);
    }

    Vect<FixedPoint<-8>, 2u> const & getPosition() const noexcept
    {
      return position;
    }

    Vect<FixedPoint<-8>, 2u> const &getSize() const noexcept
    {
      return size;
    }
  };
};
