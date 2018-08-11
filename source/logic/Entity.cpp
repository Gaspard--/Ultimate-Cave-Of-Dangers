#include "logic/Entity.hpp"
#include "logic/Logic.hpp"

namespace logic
{

  Entity::Entity()
  {
  }

  void Entity::update(Logic &)
  {
  }

  bool Entity::shouldBeRemoved() const
  {
    return false;
  }
}
