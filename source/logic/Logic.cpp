#include <algorithm>

#include "logic/Logic.hpp"

namespace logic
{
  Logic::Logic()
  {
  }

  void Logic::update()
  {
    for (Entity &entity : entities)
      entity.update(*this);
    entities.erase(std::remove_if(entities.begin(), entities.end(), [](Entity &entity) noexcept
				  {
				    return entity.shouldBeRemoved();
				  }), entities.end());
  }
}
