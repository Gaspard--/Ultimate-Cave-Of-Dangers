#pragma once

#include <array>
#include <iostream>

#include "Cave_chunk.hpp"

class CaveMap
{
private:
  std::array<CaveChunk, 10> chunks;
  size_t firstChunk;
  Tile outOfBounds;

public:
  CaveMap();
  void genNextChunk();

  CaveChunk &getChunk(size_t index) noexcept;
  CaveChunk const &getChunk(size_t index) const noexcept;

  Tile &getTile(Vect<unsigned int, 2> position) noexcept;
  Tile const &getTile(Vect<unsigned int, 2> position) const noexcept;
};
