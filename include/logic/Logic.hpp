#pragma once

#include <vector>
#include <variant>

#include "logic/Entity.hpp"

#include "Cave_map.hpp"

#include <SFML/Graphics.hpp>

namespace logic
{
  class Entity;

  struct Playing
  {
  };

  struct Pause
  {
  };

  struct GameOver
  {
  };

  static constexpr FixedPoint<-8> WaterDropSize{FixedPoint<-8>::One};

  struct WaterDrop
  {
    Vect<FixedPoint<-8>, 2u> position;
    Vect<FixedPoint<-16>, 2u> speed;

    void update() noexcept
    {
      position += speed;
      std::cout << "pos[0]: " << position[0].getFloatValue() << std::endl;
      std::cout << "speed[0]: " <<  speed[0].getFloatValue() << std::endl;
    }
  };

  class Logic
  {
  private:
    std::vector<Entity> entities;
    std::vector<WaterDrop> waterDrops;
    Vect<FixedPoint<-8>, 2u> cameraPosition;

    std::variant<Playing, Pause, GameOver> state;
    CaveMap caveMap;
  public:
    Logic();

    void update();
    void handleEvent(const sf::Event& e);
    void handleEvent(GameOver &, sf::Event const &);
    void handleEvent(Playing &, sf::Event const &);
    void handleEvent(Pause &, sf::Event const &);

    Entity &getPlayer() noexcept;
    Entity const &getPlayer() const noexcept;

    CaveMap &getMap() noexcept;
    
    CaveMap const &getMap() const noexcept;
    std::vector<WaterDrop> const &getWaterDrops() const noexcept
    {
      return waterDrops;
    }

    std::variant<Playing, Pause, GameOver> const &getState() const noexcept;
    std::vector<Entity> const &getEntities() const noexcept;

    Vect<FixedPoint<-8>, 2u> getCameraPosition() const noexcept
    {
      return cameraPosition;
    }
  };
};
