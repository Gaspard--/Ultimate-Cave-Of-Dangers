#include <iostream>

#include "Cave_chunk.hpp"

void Cave_chunk::init(Cave_chunk const &parent)
{
  unsigned int nb_room = 0;

  for (auto &pos : parent.entry_points)
    {
      chunk[pos[0]][pos[1]].type = tile_type::Entry;
    }
}

const Vect<unsigned int, 2u> &Cave_chunk::get_pos() const
{
  return pos;
}

Cave_tile const &Cave_chunk::get_tile(unsigned int x, unsigned int y) const
{
  return chunk[y][x];
}

Cave_tile &Cave_chunk::get_tile(unsigned int x, unsigned int y)
{
  return chunk[y][x];
}

void Cave_chunk::print() const
{
  for (auto &line : chunk)
    {
      for (auto &tile : line)
	{
	  switch (tile.type)
	    {
	    case tile_type::None:
	      std::cout << ' ';
	      break;
	    case tile_type::Wall:
	      std::cout << 'W';
	      break;
	    case tile_type::Entry:
	      std::cout << 'E';
	      break;
	    case tile_type::Platform:
	      std::cout << '_';
	      break;
	    default:
	      std::cout << '*';
	      break;
	    }
	  std::cout << std::endl;
	}
    }
}
