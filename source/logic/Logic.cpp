#include <algorithm>

#include "logic/Logic.hpp"
#include "logic/Physics.hpp"

namespace logic
{
  template<class Func>
  inline bool keyIsRight(Func check)
  {
    return check(sf::Keyboard::Key::D) || check(sf::Keyboard::Key::Right);
  }

  template<class Func>
  inline bool keyIsLeft(Func check)
  {
    return check(sf::Keyboard::Key::Q) || check(sf::Keyboard::Key::A) || check(sf::Keyboard::Key::Left);
  }

  Logic::Logic()
    : state(Playing{})
  {
    entities.push_back({disp::TextureList::BOB, 20});
    for (unsigned int i(0u); i < 4; ++i)
      for (unsigned int j(0u); j < 4; ++j)
	waterDrops.push_back({{FixedPoint<-8>(i * 128), FixedPoint<-8>(j * 128)},
	      {FixedPoint<-16, int>(i), FixedPoint<-16, int>(j)}});
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
	if (keyIsRight([](auto key){
	      return sf::Keyboard::isKeyPressed(key);
	    }))
	  getPlayer().drift(1);
	if (keyIsLeft([](auto key){
	      return sf::Keyboard::isKeyPressed(key);
	    }))
	  getPlayer().drift(-1);
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
	if (!waterDrops.empty())
	  checkCollisions(waterDrops.begin(), waterDrops.end());
	for (WaterDrop &waterDrop : waterDrops)
	  waterDrop.update();
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
	else if (keyIsRight([&ev](auto key)
		 {
		   return ev.key.code == key;
		 }))
	  getPlayer().dash(1);
	else if (keyIsLeft([&ev](auto key)
		 {
		   return ev.key.code == key;
		 }))
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
