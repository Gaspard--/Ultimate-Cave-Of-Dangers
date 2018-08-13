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
    Vect<FixedPoint<-16, int>, 2u> speed;
    Vect<FixedPoint<-8>, 2u> size;
    unsigned int grounded;
    Vect<unsigned int, 2u> onWall;
    disp::TextureList texture;
    sf::Clock timer;

    Vect<unsigned, 2u> hps;
  public:
    Entity(Vect<FixedPoint<-8>, 2u> position, disp::TextureList, unsigned maxHps = 0);

    void updatePosition(Logic &);
    void update(Logic &);
    void jump() noexcept;
    void drift(int dir) noexcept;
    void dash(int dir) noexcept;
    bool shouldBeRemoved() const noexcept;

    Vect<FixedPoint<-16, int>, 2u> const& getSpeed() const noexcept;
    sf::Clock const& getTimer() const noexcept;

    Vect<unsigned, 2u> const& getHps() const noexcept;
    Vect<unsigned, 2u>& getHps() noexcept;

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
