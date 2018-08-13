#include <iostream>
#include <cstdlib>

#include "Cave_chunk.hpp"
#include "logic/Logic.hpp"

void CaveChunk::init(Vect<unsigned int, 2u> pos) noexcept
{
  srand(time(nullptr));
  this->pos = pos;
  for (auto &line : tiles)
    for (auto &tile : line)
      tile = Tile(TileType::Wall);
  exitDir = ExitDirection::Right;
  for (unsigned int i(0u); i < 5u; ++i)
    for (Tile &tile : tiles[i])
      tile = Tile(TileType::Empty);
}

void CaveChunk::init(CaveChunk const &previous, logic::Logic &logic) noexcept
{
  auto nextRandomNumber([]() -> unsigned int
			{
			  return random();
			});
  for (auto &line : tiles)
    for (auto &tile : line)
      tile = Tile(TileType::Wall);
  pos = previous.pos;
  ExitDirection currentDir = previous.exitDir;
  Vect<unsigned int, 2> lowPos(0u, 0u);
  unsigned int thickness(0u);

  switch (previous.exitDir)
    {
    case ExitDirection::Left:
      lowPos[0] = CHUNK_SIZE - 1;
      while (previous.getTile({0, lowPos[1]}).type == TileType::Wall)
	++lowPos[1];
      while (previous.getTile({0, lowPos[1] + thickness}).type != TileType::Wall)
	++thickness;
      pos[0] -= 1;
      break;
    case ExitDirection::Up:
      lowPos[0] = 0;
      pos[1] += 1;
      lowPos[1] = 0;
      while (previous.getTile({lowPos[0], CHUNK_SIZE - 1}).type == TileType::Wall)
	++lowPos[0];
      while (previous.getTile({lowPos[0] + thickness, CHUNK_SIZE - 1}).type != TileType::Wall)
	++thickness;
      break;
    case ExitDirection::Right:
      lowPos[0] = 0;
      pos[0] += 1;
      while (previous.getTile({CHUNK_SIZE - 1, lowPos[1]}).type == TileType::Wall)
	++lowPos[1];
      while (previous.getTile({CHUNK_SIZE - 1, lowPos[1] + thickness}).type != TileType::Wall)
	++thickness;
      break;
    }
  {
    bool last = false;
    while (true)
      {
	unsigned int target((nextRandomNumber() % (CHUNK_SIZE - 4) + 4));
	switch (currentDir)
	  {
	  case ExitDirection::Left:
	    if (target >= lowPos[0])
	      {
		target = lowPos[0];
		last = true;
	      }
	    for (unsigned int i(0u); i < target + 1; ++i)
	      for (unsigned int j(0u); j < thickness; ++j)
		getTile(lowPos + Vect<unsigned int, 2u>{-i, j}) = Tile(TileType::Empty);
	    break;
	  case ExitDirection::Up:
	    if (target >= CHUNK_SIZE - lowPos[1])
	      {
		target = CHUNK_SIZE - lowPos[1];
		last = true;
	      }
	    {
	      int lastPlateForm(0u);
	      for (unsigned int i(0u); i < target; ++i)
		{
		  for (unsigned int j(0u); j < thickness; ++j)
		    getTile(lowPos + Vect<unsigned int, 2u>{j, i}) = Tile(TileType::Empty);
		  if (i >= lastPlateForm + 3 && i <= target - 3 && (nextRandomNumber() % (thickness) > 7))
		    {
		      lastPlateForm = i;
		      unsigned int length(nextRandomNumber() % thickness);
		      unsigned int begin(nextRandomNumber() % (thickness - length));
		      for (unsigned int j(begin); j < begin + length; ++j)
			getTile(lowPos + Vect<unsigned int, 2u>{j, i}) = Tile(TileType::Platform);
		      logic.spawnZombie(pos * CHUNK_SIZE + lowPos + Vect<unsigned int, 2u>{begin, i});
		    }
		}
	    }
	    break;
	  case ExitDirection::Right:
	    if (target >= CHUNK_SIZE - lowPos[0])
	      {
		target = CHUNK_SIZE - lowPos[0];
		last = true;
	      }
	    for (unsigned int i(0u); i < target; ++i)
	      for (unsigned int j(0u); j < thickness; ++j)
		getTile(lowPos + Vect<unsigned int, 2u>{i, j}) = Tile(TileType::Empty);
	    break;
	  }
	if (last)
	  break;
	switch (currentDir)
	  {
	  case ExitDirection::Left:
	    lowPos[0] -= target;
	    lowPos[1] += thickness;
	    currentDir = ExitDirection::Up;
	    thickness = nextRandomNumber() % (target - 3) + 3;
	    break;
	  case ExitDirection::Right:
	    lowPos[0] += target;
	    lowPos[1] += thickness;
	    currentDir = ExitDirection::Up;
	    thickness = nextRandomNumber() % (target - 3) + 3;
	    lowPos[0] -= thickness;
	    break;
	  case ExitDirection::Up:
	    lowPos[1] += target;
	    if (previous.exitDir == ExitDirection::Up)
	      currentDir = (nextRandomNumber() & 1) ? ExitDirection::Right : ExitDirection::Left;
	    else
	      currentDir = previous.exitDir;
	    if (currentDir == ExitDirection::Right)
	      lowPos[0] += thickness;
	    thickness = nextRandomNumber() % (target - 3) + 3;
	    lowPos[1] -= thickness;
	    break;
	  }
      }
    exitDir = currentDir;
  }
}

void CaveChunk::print() const
{
  for (auto &line : tiles)
    {
      for (auto &tile : line)
	{
	  switch (tile.type)
	    {
	    case TileType::Empty:
	      std::cout << ' ';
	      break;
	    case TileType::Wall:
	      std::cout << 'W';
	      break;
	    case TileType::Platform:
	      std::cout << '_';
	      break;
	    default:
	      std::cout << '*';
	      break;
	    }
	}
      std::cout << std::endl;
    }
}
