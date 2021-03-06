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

  enum class EntityType
    {
      Player,
      Zombie,
      Pickup,
      Table
    };

  class Entity
  {
    Vect<FixedPoint<-8>, 2u> position;
    Vect<FixedPoint<-16, int>, 2u> speed;
    Vect<FixedPoint<-8>, 2u> size;
    unsigned int grounded;
    Vect<unsigned int, 2u> onWall;
    disp::TextureList texture;
    sf::Clock timer;
    Vect<int, 2u> hps;
    int dir;
  public:
    EntityType type;

    Entity(Vect<FixedPoint<-8>, 2u> position, disp::TextureList, EntityType type, unsigned maxHps = 0);

    void updatePosition(Logic &);
    void update(Logic &);
    void jump() noexcept;
    void shoot(Logic &);
    void drift(int dir) noexcept;
    void dash(int dir) noexcept;
    bool shouldBeRemoved() const noexcept;

    Vect<FixedPoint<-16, int>, 2u> const& getSpeed() const noexcept;

    Vect<FixedPoint<-16, int>, 2u> & getSpeed() noexcept
    {
      return speed;
    }

    sf::Clock const& getTimer() const noexcept;

    void setGrounded(unsigned int value) noexcept
    {
      grounded = value;
    }

    int getDir() const noexcept
    {
      return dir;
    }

    void setDir(int dir) noexcept
    {
      this->dir = dir;
    }

    Vect<int, 2u> const& getHps() const noexcept;
    Vect<int, 2u>& getHps() noexcept;

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

    bool floats() const noexcept
    {
      return type == EntityType::Pickup || type == EntityType::Table;
    }
  };
};
