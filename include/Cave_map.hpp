#pragma once

#include <array>
#include <iostream>

#include "Cave_chunk.hpp"

namespace logic
{
  class Logic;
}

class CaveMap
{
private:
  std::array<CaveChunk, 10> chunks;
  size_t firstChunk;
  Tile outOfBounds;

  static size_t lastUsedChunk;
public:
  CaveMap(logic::Logic &logic);
  void genNextChunk(logic::Logic &logic);

  CaveChunk &getChunk(size_t index) noexcept;
  CaveChunk const &getChunk(size_t index) const noexcept;
  void regenIfNecessary(Vect<unsigned int, 2> position, logic::Logic &logic) noexcept;

  Tile &getTile(Vect<unsigned int, 2> position, size_t & = lastUsedChunk) noexcept;
  Tile const &getTile(Vect<unsigned int, 2> position, size_t & = lastUsedChunk) const noexcept;
};
