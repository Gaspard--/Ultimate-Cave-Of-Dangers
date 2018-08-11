#pragma once

#define CHUNK_SIZE 256
#define ROOM_LIMITE 10

#include <array>
#include <iostream>
#include <vector>

#include "Map_tile.hpp"
#include "Vect.hpp"

enum class exit_direction : char
{
	Ouest = 0,
	Nord,
	Est,
	South // shouldn't be possible
};

class Cave_chunk
{
private:
  std::array<std::array<Cave_tile, CHUNK_SIZE>, CHUNK_SIZE> chunk;
  std::vector<Vect<unsigned int, 2u>> entry_points;
  Vect<unsigned int, 2> pos;

public:
  Cave_chunk() = default;
  Cave_chunk(const std::vector<Vect<int, 2u>> &entry_points);

  void init(const Cave_chunk &entry_points);
  const Vect<unsigned int, 2u> &get_pos() const;
  Cave_tile const &get_tile(unsigned int x, unsigned int y) const;
  Cave_tile &get_tile(unsigned int x, unsigned int y);
  void print() const;
};
