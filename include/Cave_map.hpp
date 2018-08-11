#pragma once

#include <array>
#include <iostream>

#include "Cave_chunk.hpp"

class Cave_map
{
private:
  std::array<Cave_chunk, 10> chunks;
  size_t firstChunk;
  Cave_tile outOfBounds;

public:
  Cave_map();
  void genNextChunk();

  Cave_chunk &getChunk(size_t index) noexcept;
  Cave_chunk const &getChunk(size_t index) const noexcept;

  Cave_tile &getTile(Vect<unsigned int, 2> position) noexcept;
  Cave_tile const &getTile(Vect<unsigned int, 2> position) const noexcept;
};
