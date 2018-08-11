#pragma once

#include <vector>
#include <variant>

#include "logic/Entity.hpp"

#include <SFML/Graphics.hpp>

namespace logic
{
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

    std::variant<Playing, Pause, GameOver> state;
  public:
    Logic();

    void update();
    void handleEvent(const sf::Event& e);
    void handleEvent(GameOver &, sf::Event const &);
    void handleEvent(Playing  &, sf::Event const &);
    void handleEvent(Pause &, sf::Event const &);

    std::variant<Playing, Pause, GameOver> const &getState() const noexcept;
    std::vector<Entity> const &getEntities() const noexcept;
  };
};
