#include <algorithm>

#include "logic/Logic.hpp"

namespace logic
{
  Logic::Logic()
  {
  }

  Entity &Logic::getPlayer() noexcept
  {
    return entities.front();
  }

  Entity const &Logic::getPlayer() const noexcept
  {
    return entities.front();
  }

  void Logic::update()
  {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
      {
	getPlayer().drift(-1);
      }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
      {
	getPlayer().drift(1);
      }
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

  void Logic::handleEvent(Playing  &, sf::Event const &ev)
  {
    switch (ev.type)
      {
      case sf::Event::KeyPressed:
	switch (ev.key.code)
	  {
	  case sf::Keyboard::Key::W:
	    getPlayer().jump();
	    break;
	  case sf::Keyboard::Key::A:
	    getPlayer().dash(-1);
	    break;
	  case sf::Keyboard::Key::D:
	    getPlayer().dash(1);
	    break;
	  default:
	    break;
	  }
	break;
      default:
	break;
      }
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
