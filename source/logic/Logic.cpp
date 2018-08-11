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

  void Logic::handleEvent(const sf::Event& e)
  {
    std::visit([&](auto &state)
	       {
		 handleEvent(state, e);
	       }, state);
  }

  void Logic::handleEvent(GameOver &, sf::Event const &)
  {
  }

  void Logic::handleEvent(Playing  &, sf::Event const &)
  {
  }

  void Logic::handleEvent(Pause &, sf::Event const &)
  {
  }

  std::variant<Playing, Pause, GameOver> const &Logic::getState() const noexcept
  {
    return state;
  }

  std::vector<Entity> const &Logic::getEntities() const noexcept
  {
    return entities;
  }

  Vect<FixedPoint<-8>, 2u> const &Logic::getPlayerPosition() const
  {
    return Vect<FixedPoint<-8>, 2u>();
  }
}
