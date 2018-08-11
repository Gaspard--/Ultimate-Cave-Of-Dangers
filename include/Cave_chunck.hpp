#pragma once

#define CHUNCK_SIZE 256
#define ROOM_LIMITE 10

#include <array>
#include <iostream>

#include "Map_tile.hpp"

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
	std::array<std::array<tile, CHUNCK_SIZE>, CHUNCK_SIZE> chunck;
	std::vector<int, 2u> *entry_point;
	std::vector<int, 2> pos;

public:
	Cave_chunck(const unsigned int[2] *entry_point);
	~Cave_chunck();
	const std::vector<int, 2u> get_pos() const;
	Cave_tile const &get_tile(unsigned int x, unsigned int y) const;
	void print(); const
};
