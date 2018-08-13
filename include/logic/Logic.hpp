#pragma once

#include <vector>
#include <variant>

#include "logic/Entity.hpp"

#include "Cave_map.hpp"

#include <SFML/Graphics.hpp>

# include "logic/Anim.hpp"

namespace logic
{
  class Entity;

  struct Playing
  {
    size_t shootCooldownLeft{0u};
    bool pause{0u};
  };

  struct GameOver
  {
  };

  static constexpr FixedPoint<-8> WaterDropSize{FixedPoint<-8>::One};
  static constexpr FixedPoint<-16> SpawnPosX{60000_FP};

  class Logic
  {
  private:
    std::vector<Entity> entities;
    std::vector<Anim> animations;
    Vect<FixedPoint<-8>, 2u> cameraPosition;
    FixedPoint<-8> waterLevel;
    unsigned int waterDamageCooldown;

    std::variant<Playing, GameOver> state;
    CaveMap caveMap;
  public:
    Logic();

    void update();
    void handleEvent(const sf::Event& e);
    void handleEvent(GameOver &, sf::Event const &);
    void handleEvent(Playing &, sf::Event const &);

    Entity &getPlayer() noexcept;
    Entity const &getPlayer() const noexcept;

    CaveMap &getMap() noexcept;

    CaveMap const &getMap() const noexcept;

    FixedPoint<-8> getWaterLevel() const noexcept
    {
      return waterLevel;
    }

    std::variant<Playing, GameOver> const &getState() const noexcept;
    std::vector<Entity> const &getEntities() const noexcept;
    std::vector<Anim> const &getAnimations() const noexcept;
    std::vector<Entity> &getEntities() noexcept
    {
      return entities;
    }

    Vect<FixedPoint<-8>, 2u> getCameraPosition() const noexcept
    {
      return cameraPosition;
    }

    void removeAllEntitiesInChunk(Vect<unsigned int, 2u> chunkPos);
    void spawnZombie(Vect<unsigned int, 2u> floorPosition);
    void showHit(Vect<FixedPoint<-8>, 2u>);
  };
};
