#pragma once

namespace logic
{
  class Logic;

  class Entity
  {
  public:
    Entity();

    void update(Logic &);
    bool shouldBeRemoved() const;
  };
};
