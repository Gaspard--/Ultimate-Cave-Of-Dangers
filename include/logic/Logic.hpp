#pragma once

#include <vector>

#include "logic/Entity.hpp"

# include <SFML/Graphics.hpp>

namespace logic
{
  class Logic
  {
  private:
    std::vector<Entity> entities;

  public:
    Logic();

    void update();
    void handleEvent(const sf::Event& e);
    void checkEvent();

  };
};
