#include <iostream>

#include "Cave_chunck.hpp"

Cave_chunck::Cave_chunck(Cave_chunck const &parent)
{
  unsigned int nb_room = 0;

  for (auto &pos : parent.entry_points)
    {
      chunck[pos[0]][pos[1]].type = tile_type::Entry;
    }

  while (nb_room < ROOM_LIMITE);
}

const Vect<int, 2u> Cave_chunck::get_pos() const
{
  return pos;
}

Cave_tile const &Cave_chunck::get_tile(unsigned int x, unsigned int y) const
{
  return chunck[y][x];
}

void Cave_chunck::print() const
{
  for (auto &line : chunck)
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
