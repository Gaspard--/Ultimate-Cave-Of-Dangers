#pragma once

#define CHUNCK_SIZE 256
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

class Cave_chunck
{
private:
  std::array<std::array<Cave_tile, CHUNCK_SIZE>, CHUNCK_SIZE> chunck;
  std::vector<Vect<int, 2u>> entry_points;
  Vect<int, 2> pos;

public:
  Cave_chunck() = default;
  Cave_chunck(const std::vector<Vect<int, 2u>> &entry_points);
  explicit Cave_chunck(const Cave_chunck &entry_points);
  const Vect<int, 2u> get_pos() const;
  Cave_tile const &get_tile(unsigned int x, unsigned int y) const;
  void print() const;
};
