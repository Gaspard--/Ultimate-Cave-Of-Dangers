#include <algorithm>

#include "logic/Logic.hpp"
# include <cstdlib>

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

  template<class Func>
  inline bool keyIsPause(Func check)
  {
    return check(sf::Keyboard::Key::Escape) || check(sf::Keyboard::Key::P);
  }

  Logic::Logic()
    : cameraPosition(SpawnPosX, 0_FP)
    , waterSpeed(64)
    , state(Playing{})
    , caveMap(*this)
    , waterDamageCooldown(30)
    , score(0u)
  {
    entities.insert(entities.begin(), Entity{{FixedPoint<-16>{SpawnPosX}, FixedPoint<-16>::One}, disp::TextureList::BOB, EntityType::Player, 40});
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

  void Logic::checkInputs(Playing &state)
  {
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
	if (keyIsLeft([](auto key){
	  return sf::Keyboard::isKeyPressed(key);
	    }))
	  getPlayer().setDir(-1);
	else if (keyIsRight([](auto key){
	  return sf::Keyboard::isKeyPressed(key);
	    }))
	  getPlayer().setDir(-1);
	else if (!getPlayer().getDir())
	  getPlayer().setDir(getPlayer().getSpeed()[0].isPositive() - getPlayer().getSpeed()[0].isNegative());
	if (!state.shootCooldownLeft)
	  {
	    getPlayer().shoot(*this);
	    animations.push_back({getPlayer().getPosition() + Vect<FixedPoint<-8>, 2u>{0_uFP, 1_uFP}, 20.f, disp::TextureList::SPARKS, 5});
	    animations.push_back({getPlayer().getPosition() + Vect<FixedPoint<-8>, 2u>{getPlayer().getDir() != -1 ? 6_uFP : -2_uFP, 1_uFP}, 20.f, disp::TextureList::SHOT_LINE, 1});
	    state.shootCooldownLeft = 6;
	  }
      }
    else if (!state.shootCooldownLeft)
      {
	getPlayer().setDir(0);
      }
    state.shootCooldownLeft -= !!state.shootCooldownLeft;
  }

  void Logic::update()
  {
    std::visit([this](auto &state) noexcept(!std::is_same_v<std::decay_t<decltype(state)>, Playing>) {
	if constexpr (std::is_same_v<std::decay_t<decltype(state)>, Playing>) {
	    if (state.pause)
	      return ;
	    checkInputs(state);
	    cameraPosition = (cameraPosition * 7_uFP + getPlayer().getPosition() * 1_uFP) / 8_uFP;
	    for (auto it(entities.begin() + 1); it < entities.end(); ++it)
	      {
		Entity &entity(*it);
		if (entity.type == EntityType::Zombie)
		  {
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
	    if (waterLevel < getPlayer().getPosition()[1])
	      waterLevel += ((getPlayer().getPosition()[1] - waterLevel) / 256_uFP);
	  }
        waterLevel += FixedPoint<-8>(waterSpeed) + FixedPoint<-8>{uint32_t(sin(waterLevel.getFloatValue() * 0.1f) * 12.0f)};
	for (auto it(entities.begin() + 1); it < entities.end(); ++it)
	  {
	    Entity &entity(*it);
	    for (uint32_t i(0); i < 2; ++i)
	      {
		if (getPlayer().getPosition()[i] + getPlayer().getSize()[i] < entity.getPosition()[i] ||
		    entity.getPosition()[i] + entity.getSize()[i] < getPlayer().getPosition()[i])
		  goto skip;
	      }
	    {
	      switch (entity.type)
		{
		case EntityType::Zombie:
		  {
		    Vect<FixedPoint<-16, int>, 2u> impulse(getPlayer().getPosition() - entity.getPosition());
		    FixedPoint<-16, int> length(int(std::sqrt(float(impulse.length2().value))));
		    if (!!length)
		      {
			impulse *= 2_FP;
			impulse /= length;
		      }
		    impulse[1] += FixedPoint<-16>{16};
		    getPlayer().getSpeed() += Vect<FixedPoint<-16, int>, 2u>(impulse);
		    getPlayer().getHps()[0] -= 2;
		    showHit({getPlayer().getPosition() + getPlayer().getSize() / 2_FP});
		  }
		  break;
		case EntityType::Pickup:
		  {
		    entity.getHps()[0] = 0;
		    entity.getHps()[1] = 1;
		    getPlayer().getHps()[0] += 5;
		    if (getPlayer().getHps()[0] > getPlayer().getHps()[1])
		      getPlayer().getHps()[0] = getPlayer().getHps()[1];
		    score += 20;
		  }
		  break;
		case EntityType::Table:
		  if (getPlayer().getPosition()[1] - FixedPoint<-8>(getPlayer().getSpeed()[1]) >
		      entity.getPosition()[1] + entity.getSize()[1] - FixedPoint<-8>(entity.getSpeed()[1]))
		    {
		      getPlayer().setGrounded(10);
		      getPlayer().getSpeed()[1] = entity.getSpeed()[1] - getPlayer().getSpeed()[1] * 2_FP;
		      score += 5;
		    }
		  break;
		}
	    }
	  skip:
	    ;
	  }
	--waterDamageCooldown;
	for (Entity &entity : entities)
	  if (entity.floats() &&entity.getPosition()[1] < waterLevel)
	    entity.getSpeed()[1] += FixedPoint<-8>{9};
	  else if (!waterDamageCooldown && entity.getPosition()[1] + entity.getSize()[1] < waterLevel)
	    entity.getHps()[0] -= 1;
	if (!waterDamageCooldown)
	  {
	    waterDamageCooldown = 30;
	    waterSpeed += FixedPoint<-12>{1};
	  }
	for (Anim &anim : animations)
	  anim.update();
	for (Entity &entity : entities)
	  entity.update(*this);
	if (getPlayer().shouldBeRemoved())
	  this->state = GameOver();
	if constexpr (std::is_same_v<std::decay_t<decltype(state)>, Playing>) {
	    caveMap.regenIfNecessary({FixedPoint<0>(getPlayer().getPosition()[0]).value,
		  FixedPoint<0>(getPlayer().getPosition()[1]).value}, *this);
	  }
	entities.erase(std::remove_if(entities.begin(), entities.end(), [](Entity &entity) noexcept
				      {
					return entity.shouldBeRemoved();
				      }), entities.end());
	animations.erase(std::remove_if(animations.begin(), animations.end(), [](Anim &anim) noexcept
				      {
					return anim.shouldBeRemoved();
				      }), animations.end());
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

  void Logic::handleEvent(Playing &state, sf::Event const &ev)
  {
    switch (ev.type)
      {
      case sf::Event::KeyPressed:
	if (keyIsPause([&ev](auto key)
		       {
			 return ev.key.code == key;
		       }))
	  state.pause ^= 1;
	if (state.pause)
	  {
	  }
	else
	  {
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
	  }
	break;
      default:
	break;
      }
  }

  std::variant<Playing, GameOver> const &Logic::getState() const noexcept
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
	  disp::TextureList::ZOMBIE,
	    EntityType::Zombie, 6});
  }

  void Logic::spawnTable(Vect<unsigned int, 2u> floorPosition)
  {
    disp::TextureList texture;
    unsigned rand = random();
    texture = disp::TextureList::TABLE;
    if (rand % 3 == 1)
      texture = disp::TextureList::BARREL;
    else if (rand % 3 == 2)
      texture = disp::TextureList::MINEKART;

    entities.push_back(Entity{
	{
	  FixedPoint<-16>::One * FixedPoint<0>(floorPosition[0]),
	    FixedPoint<-16>::One  * FixedPoint<0>(floorPosition[1])
	    },
	  texture,
	    EntityType::Table, 0});
  }

  void Logic::showHit(Vect<FixedPoint<-8>, 2u> hitPosition)
  {
    animations.push_back({hitPosition, 20.f, disp::TextureList::SHOT, 5});
  }

  std::vector<Anim> const &Logic::getAnimations() const noexcept
  {
    return (animations);
  }
}
