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

  class Logic
  {
  private:
    std::vector<Entity> entities;
    Vect<FixedPoint<-8>, 2u> cameraPosition;

    std::variant<Playing, Pause, GameOver> state;
    CaveMap caveMap;
  public:
    Logic();

    void update();
    void handleEvent(const sf::Event& e);
    void handleEvent(GameOver &, sf::Event const &);
    void handleEvent(Playing  &, sf::Event const &);
    void handleEvent(Pause &, sf::Event const &);

    Entity &getPlayer() noexcept;
    Entity const &getPlayer() const noexcept;

    CaveMap &getMap() noexcept;
    CaveMap const &getMap() const noexcept;

    std::variant<Playing, Pause, GameOver> const &getState() const noexcept;
    std::vector<Entity> const &getEntities() const noexcept;

    Vect<FixedPoint<-8>, 2u> getCameraPosition() const noexcept
    {
      return cameraPosition;
    }
  };
};
