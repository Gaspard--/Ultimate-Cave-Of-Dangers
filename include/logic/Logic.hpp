#pragma once

#include <vector>

#include "logic/Entity.hpp"

namespace logic
{
  class Logic
  {
  private:
    std::vector<Entity> entities;

  public:
    Logic();

    void update();
  };
};
