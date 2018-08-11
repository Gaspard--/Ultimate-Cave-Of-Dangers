#include <iostream>

#include "Cave_chunk.hpp"

void CaveChunk::init(CaveChunk const &previous) noexcept
{
  
}

void CaveChunk::print() const
{
  for (auto &line : tiles)
    {
      for (auto &tile : line)
	{
	  switch (tile.type)
	    {
	    case TileType::None:
	      std::cout << ' ';
	      break;
	    case TileType::Wall:
	      std::cout << 'W';
	      break;
	    case TileType::Entry:
	      std::cout << 'E';
	      break;
	    case TileType::Platform:
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
