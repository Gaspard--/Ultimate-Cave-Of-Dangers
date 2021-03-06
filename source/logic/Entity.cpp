#include "logic/Entity.hpp"
#include "logic/Logic.hpp"

namespace logic
{

  Entity::Entity(Vect<FixedPoint<-8>, 2u> position, disp::TextureList texture, EntityType type, unsigned maxHps)
    : position(position)
    , size(type == EntityType::Table ? 2_uFP : 1_uFP, 2_uFP)
    , grounded(0)
    , texture(texture)
    , hps({maxHps, maxHps})
    , type(type)
    , dir(0)
  {
  }

  Vect<FixedPoint<-16, int>, 2u> const& Entity::getSpeed() const noexcept
  {
    return (speed);
  }

  sf::Clock const& Entity::getTimer() const noexcept
  {
    return (timer);
  }

  void Entity::updatePosition(Logic &logic)
  {
    if (!speed.any())
      return ;
    Vect<FixedPoint<-16, int>, 2u> delta(speed);
    auto minBound([this](size_t dir)
    {
      return FixedPoint<-24, int>((FixedPoint<-8>::One - ((position[dir] + size[dir]) % FixedPoint<-8>::One)) % FixedPoint<-8>::One);
    });
    auto maxBound([this](size_t dir)
    {
      return -FixedPoint<-24, int>(position[dir] % FixedPoint<-8>::One);
    });
    auto const TimeTileNextTile([&delta, minBound, maxBound](size_t dir) noexcept
				{
				  if (!delta[dir])
				    return FixedPoint<-8, int>::One;
				  if (delta[dir].isPositive())
				    return minBound(dir) / delta[dir];
				  else
				    return maxBound(dir) / delta[dir];
				});
    auto move([this, &delta, minBound, maxBound](size_t dir)
	      {
		auto moveInDir(delta[dir].isPositive() ?
			       minBound(dir) :
			       maxBound(dir));
	        auto moveInOther(moveInDir * delta[!dir] / delta[dir]);

		position[dir] += moveInDir;
		position[!dir] += moveInOther;
		delta[dir] -= moveInDir;
		delta[!dir] -= moveInOther;
	      });
    while (delta.any())
      {
	auto timeX(TimeTileNextTile(0));
	auto timeY(TimeTileNextTile(1));

	if (timeX >= decltype(timeX)::One && timeY >= decltype(timeY)::One)
	  {
	    position += delta;
	    return ;
	  }
	if (timeX < timeY)
	  {
	    move(0);
	    unsigned int x(FixedPoint<0>(position[0] + (delta[0].isPositive() ? size[0] : FixedPoint<-8>::Zero)).value - !delta[0].isPositive());
	    unsigned int minY(FixedPoint<0>(position[1]).value);
	    unsigned int maxY(FixedPoint<0>(position[1] + size[1]).value + !!((position[1] + size[1]) % FixedPoint<-8, int>::One));
	    for (unsigned int y(minY); y != maxY; ++y)
	      {
		if (logic.getMap().getTile({x, y}).isSolid())
		  {
		    onWall[!delta[0].isPositive()] = 20;
		    delta[0] = FixedPoint<-16>::Zero;
		    speed[0] = FixedPoint<-16>::Zero;
		    goto outX;
		  }
	      }
	    position[0] += FixedPoint<-8>{speed[0].isPositive() * 2 - 1};
	  outX:
	    ;
	  }
	else
	  {
	    move(1);
	    unsigned int y(FixedPoint<0>(position[1] + (delta[1].isPositive() ? size[1] : FixedPoint<-8>::Zero)).value - delta[1].isNegative());
	    unsigned int minX(FixedPoint<0>(position[0]).value);
	    unsigned int maxX(FixedPoint<0>(position[0] + size[0]).value + !!((position[0] + size[0]) % FixedPoint<-8, int>::One));
	    for (unsigned int x(minX); x != maxX; ++x)
	      {
		Tile const &tile(logic.getMap().getTile({x, y}));
		if (tile.isSolid() || (delta[1].isNegative() && tile.isSolidAbove()))
		  {
		    delta[1] = FixedPoint<-16>::Zero;
		    speed[1] = FixedPoint<-16>::Zero;
		    if (!speed[1].isPositive())
		      grounded = 5;
		    goto outY;
		  }
	      }
	    position[1] += FixedPoint<-8>{speed[1].isPositive() * 2 - 1};
	  outY:
	    ;
	  }
      }
  }

  void Entity::update(Logic &logic)
  {
    updatePosition(logic);
    speed[1] -= FixedPoint<-8, int>{3};
    if (onWall.any())
      {
	speed[1] *= FixedPoint<-4, int>{15};
      }
    grounded -= !!grounded;
    onWall -= !!onWall;
    speed *= FixedPoint<-6, int>{63};
    if (type == EntityType::Zombie && hps[0] <= 0)
      {
	type = EntityType::Pickup;
	texture = disp::TextureList::HEALTH;
	size = {FixedPoint<-8>::One / 2_uFP, FixedPoint<-8>::One / 2_uFP};
	hps = {0, 0};
	speed[1] += FixedPoint<-8>{30};
	logic.score += 10;
      }
  }

  void Entity::jump() noexcept
  {
    if (grounded)
      {
	speed[1] = FixedPoint<-8, int>{128};
	grounded = false;
      }
    else if (onWall.any())
      {
	speed[0] = -FixedPoint<-8, int>{(!onWall[1] - !onWall[0]) * 192};
	speed[1] = FixedPoint<-8, int>{120};
	onWall = {false, false};
      }
  }

  void Entity::drift(int dir) noexcept
  {
    speed[0] += FixedPoint<-10, int>{dir * 8};
  }

  void Entity::dash(int dir) noexcept
  {
    if (grounded && speed[0].isPositive() - speed[0].isNegative() != dir)
      {
	speed[0] += FixedPoint<-8, int>{dir * 12};
	speed[1] += FixedPoint<-8, int>{4};
      }
  }

  bool Entity::shouldBeRemoved() const noexcept
  {
    if (hps[1] && hps[0] <= 0)
      return true;
    return false;
  }

  Vect<int, 2u> const& Entity::getHps() const noexcept
  {
    return hps;
  }

  Vect<int, 2u>& Entity::getHps() noexcept
  {
    return hps;
  }

  void Entity::shoot(Logic &logic)
  {
    FixedPoint<0, int> dir(this->dir);
    FixedPoint<-8> closestPos(64_uFP);
    FixedPoint<-8> height(position[1] + FixedPoint<-8>::One);

    auto bestIt(logic.getEntities().begin());
    for (auto it(logic.getEntities().begin() + 1); it < logic.getEntities().end(); ++it)
      {
	Entity &entity(*it);
	if (entity.type == EntityType::Zombie || entity.type == EntityType::Table) {
	  if (entity.getPosition()[1] < height &&
	      entity.getPosition()[1] + entity.getSize()[1] > height)
	    {
	      FixedPoint<-8, int> diff(FixedPoint<-8, int>(entity.getPosition()[0] - position[0]) * dir);

	      if (diff.isPositive() && diff < closestPos)
		{
		  bestIt = it;
		  closestPos = diff;
		}
	    }
	}

      }
    if (bestIt != logic.getEntities().begin())
      {
	bestIt->speed[0] += FixedPoint<-8, int>(32) * dir;
	bestIt->speed[1] += FixedPoint<-8, int>(16);
	if (bestIt->getHps()[1])
	  {
	    bestIt->getHps()[0] -= 1;
	    bestIt->setDir(-this->dir);
	    logic.score += 5;
	  }
      }
    std::cout << "shooting" << std::endl;
    speed[0] -= FixedPoint<-8, int>(16) * dir;
    logic.showHit({position[0] + FixedPoint<-8, int>(closestPos) * dir, height});
  }
}
