#include <algorithm>

#include "logic/Logic.hpp"

namespace logic
{
  Logic::Logic()
    : state(Playing{})
  {
    entities.push_back(disp::TextureList::BOB);
  }

  Entity &Logic::getPlayer() noexcept
  {
    return entities.front();
  }

  Entity const &Logic::getPlayer() const noexcept
  {
    return entities.front();
  }

  CaveMap &Logic::getMap() noexcept
  {
    return (caveMap);
  }

  CaveMap const &Logic::getMap() const noexcept
  {
    return (caveMap);
  }

  void Logic::update()
  {
    if (std::holds_alternative<Playing>(state))
      {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	  getPlayer().drift(-1);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	  getPlayer().drift(1);
	cameraPosition = cameraPosition * FixedPoint<-3>(7) + getPlayer().getPosition()  * FixedPoint<-3>(1);
      }
    if (!std::holds_alternative<Pause>(state))
      {
	for (Entity &entity : entities)
	  entity.update(*this);
	entities.erase(std::remove_if(entities.begin(), entities.end(), [](Entity &entity) noexcept
				      {
					return entity.shouldBeRemoved();
				      }), entities.end());
      }
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
	if (ev.key.code == sf::Keyboard::Key::W || ev.key.code == sf::Keyboard::Key::Space || ev.key.code == sf::Keyboard::Key::Z || ev.key.code == sf::Keyboard::Key::Up)
	  getPlayer().jump();
	else if (ev.key.code == sf::Keyboard::Key::D || ev.key.code == sf::Keyboard::Key::Right)
	  getPlayer().dash(1);
	else if (ev.key.code == sf::Keyboard::Key::A || ev.key.code == sf::Keyboard::Key::Q || ev.key.code == sf::Keyboard::Key::Left)
	  getPlayer().dash(-1);
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
}
