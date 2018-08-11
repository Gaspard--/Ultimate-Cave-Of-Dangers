#pragma once

#define CHUNK_SIZE 256
#define ROOM_LIMITE 10

#include <array>
#include <iostream>
#include <vector>

#include "Map_tile.hpp"
#include "Vect.hpp"

enum class ExitDirection : unsigned char
  {
    Left = 0,
    Up,
    Right,
  };

class CaveChunk
{
private:
  std::array<std::array<CaveTile, CHUNK_SIZE>, CHUNK_SIZE> tiles;
  Vect<unsigned int, 2> pos;
  ExitDirection exitDir;

public:
  CaveChunk() = default;

  void init(const CaveChunk &previous) noexcept;

  const Vect<unsigned int, 2u> &getPos() const noexcept
  {
    return pos;
  }

  CaveTile const &getTile(unsigned int x, unsigned int y) const noexcept
  {
    return tiles[y][x];
  }

  CaveTile &getTile(unsigned int x, unsigned int y) noexcept
  {
    return tiles[y][x];
  }

  void print() const;
};
