#include "logic/Entity.hpp"
#include "logic/Logic.hpp"

namespace logic
{

  Entity::Entity(disp::TextureList texture)
    : position(FixedPoint<0>(1), FixedPoint<0>(2))
    , size(FixedPoint<0>(1), FixedPoint<0>(2))
    , grounded(0)
    , texture(texture)
  {
  }

  void Entity::update(Logic &)
  {
    position += speed;
    if (!grounded)
      {
	speed[1] -= FixedPoint<-8, int>{8};
      }
    if (onWall.any())
      {
	speed[1] *= FixedPoint<-4, int>{15};
      }
    grounded -= !!grounded;
    onWall -= !!onWall;
    speed *= FixedPoint<-6, int>{63};
    grounded = 2;
  }

  void Entity::jump() noexcept
  {
    if (grounded)
      {
	speed[1] = FixedPoint<-8, int>{256};
      }
    else if (onWall.any())
      {
	speed[0] = FixedPoint<-8, int>{(!onWall[1] - !onWall[0]) * 16};
	speed[1] = FixedPoint<-8, int>{128};
      }
  }

  void Entity::drift(int dir) noexcept
  {
    speed[0] += FixedPoint<-8, int>{dir};
  }

  void Entity::dash(int dir) noexcept
  {
    if (grounded)
      {
	speed[0] += FixedPoint<-8, int>{dir * 8};
	speed[1] += FixedPoint<-8, int>{32};
      }
  }

  bool Entity::shouldBeRemoved() const noexcept
  {
    return false;
  }
}
