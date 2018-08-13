#pragma once

#define CHUNK_SIZE 32
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
  std::array<std::array<Tile, CHUNK_SIZE>, CHUNK_SIZE> tiles;
  Vect<unsigned int, 2> pos;
  ExitDirection exitDir;

public:
  CaveChunk() = default;

  void init(Vect<unsigned int, 2u> previous) noexcept;
  void init(const CaveChunk &previous) noexcept;

  const Vect<unsigned int, 2u> &getPos() const noexcept
  {
    return pos;
  }

  Tile const &getTile(Vect<unsigned int, 2u> pos) const noexcept
  {
    return tiles[pos[1]][pos[0]];
  }

  Tile &getTile(Vect<unsigned int, 2u> pos) noexcept
  {
    return tiles[pos[1]][pos[0]];
  }

  void print() const;
};
