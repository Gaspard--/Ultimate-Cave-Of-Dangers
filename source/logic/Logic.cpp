#include <algorithm>

#include "logic/Logic.hpp"

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
    , caveMap(*this)
  {
    entities.insert(entities.begin(), Entity{{FixedPoint<-16>{SpawnPosX}, FixedPoint<-16>::One}, disp::TextureList::BOB, 20});
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
    std::visit([this](auto &state) noexcept(!std::is_same_v<std::decay_t<decltype(state)>, Playing>) {
	if constexpr (std::is_same_v<std::decay_t<decltype(state)>, Playing>) {
	    if (keyIsRight([](auto key){
		  return sf::Keyboard::isKeyPressed(key);
		}))
	      getPlayer().drift(3);
	    if (keyIsLeft([](auto key){
		  return sf::Keyboard::isKeyPressed(key);
		}))
	      getPlayer().drift(-3);
	    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
	      {
		if (!getPlayer().getDir())
		  getPlayer().setDir(getPlayer().getSpeed()[0].isPositive() - getPlayer().getSpeed()[0].isNegative());
		if (!state.shootCooldownLeft)
		  {
		    getPlayer().shoot(*this);
		    state.shootCooldownLeft = 6;
		  }
	      }
	    else if (!state.shootCooldownLeft)
	      {
		getPlayer().setDir(0);
	      }
	    
	    state.shootCooldownLeft -= !!state.shootCooldownLeft;
	    cameraPosition = (cameraPosition * 7_uFP + getPlayer().getPosition() * 1_uFP) / 8_uFP;
	    for (auto it(entities.begin() + 1); it < entities.end(); ++it)
	      {
		Entity &entity(*it);
		int dir(entity.getSpeed()[0].isPositive() * 2 - 1);
		if (int faceDir = entity.getDir())
		  {
		    entity.drift(faceDir);
		    if (faceDir != dir)
		      entity.dash(faceDir);
		    else
		      entity.setDir(0);
		  }
		else
		  {
		    FixedPoint<0> nextX((FixedPoint<0>(entity.getPosition()[0] + FixedPoint<-8, int>(entity.getSpeed()[0]) * 5_FP) + FixedPoint<0>(entity.getSpeed()[0].isPositive())).value);

		    if (!caveMap.getTile({nextX.value, FixedPoint<0>(entity.getPosition()[1]).value}).isSolid() && // turn around if running into a wall
			caveMap.getTile({nextX.value, (FixedPoint<0>(entity.getPosition()[1]) - 1_uFP).value}).isSolidAbove()) // turn around if about to fall
		      {
			entity.drift(dir);
		      }
		    else
		      {
			entity.dash(-dir);
		      }
		  }
	      }
	  }
        if constexpr (!std::is_same_v<std::decay_t<decltype(state)>, Pause>) {
	    waterLevel += FixedPoint<-8>{8u + uint32_t(sin(waterLevel.getFloatValue() * 0.1f) * 4.0f)};
	    if (waterLevel < getPlayer().getPosition()[1])
	      waterLevel += ((getPlayer().getPosition()[1] - waterLevel) * 1_uFP / 128_uFP);
	    std::cout << "Water level: " << waterLevel.getFloatValue() << std::endl;
	    for (Entity &entity : entities)
	      entity.update(*this);
	    entities.erase(std::remove_if(entities.begin(), entities.end(), [](Entity &entity) noexcept
					  {
					    return entity.shouldBeRemoved();
					  }), entities.end());
	    caveMap.regenIfNecessary({FixedPoint<0>(getPlayer().getPosition()[0]).value,
		  FixedPoint<0>(getPlayer().getPosition()[1]).value}, *this);
	  }
      }, state);
  }

  void Logic::removeAllEntitiesInChunk(Vect<unsigned int, 2u> chunkPos)
  {
    Vect<FixedPoint<-8>, 2u> min(FixedPoint<0>(chunkPos[0] * CHUNK_SIZE), FixedPoint<0>(chunkPos[1] * CHUNK_SIZE));
    Vect<FixedPoint<-8>, 2u> max(FixedPoint<0>((chunkPos[0] + 1) * CHUNK_SIZE), FixedPoint<0>((chunkPos[1] + 1) * CHUNK_SIZE));
    entities.erase(std::remove_if(entities.begin(), entities.end(), [min, max](Entity &entity) noexcept
				  {
				    return (entity.getPosition()[0] > min[0] &&
					    entity.getPosition()[1] > min[1] &&
					    entity.getPosition()[0] > max[0] &&
					    entity.getPosition()[1] > max[1]);
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
	if (ev.key.code == sf::Keyboard::Key::W || ev.key.code == sf::Keyboard::Key::Z || ev.key.code == sf::Keyboard::Key::Up)
	  getPlayer().jump();
	else if (keyIsRight([&ev](auto key)
			    {
			      return ev.key.code == key;
			    }))
	  getPlayer().dash(3);
	else if (keyIsLeft([&ev](auto key)
			   {
			     return ev.key.code == key;
			   }))
	  getPlayer().dash(-3);
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

  void Logic::spawnZombie(Vect<unsigned int, 2u> floorPosition)
  {
    entities.push_back(Entity{
	{
	  FixedPoint<-16>::One * FixedPoint<0>(floorPosition[0]),
	    FixedPoint<-16>::One  * FixedPoint<0>(floorPosition[1] + 1)
	    },
	  disp::TextureList::ZOMBIE, 20});
  }
}
