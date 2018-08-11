#include "logic/Entity.hpp"
#include "logic/Logic.hpp"

namespace logic
{

  Entity::Entity()
  {
  }

  void Entity::update(Logic &)
  {
    position += speed;
    if (!grounded)
      {
	speed[1] -= FixedPoint<-8>{32};
      }
    if (onWall.any())
      {
	speed[1] *= FixedPoint<-4>{15};
      }
    grounded -= !!grounded;
    onWall -= !!onWall;
  }

  void Entity::jump() noexcept
  {
    if (grounded)
      {
	speed[0] = FixedPoint<-8>{256};
      }
    else if (onWall.any())
      {
	speed[0] = FixedPoint<-8>{128};
	speed[1] = FixedPoint<-8>{(!onWall[1] - !onWall[0]) * 32};
      }
  }

  bool Entity::shouldBeRemoved() const noexcept
  {
    return false;
  }
}
