#include <cassert>

#include "Cave_map.hpp"
#include "logic/Logic.hpp"

size_t CaveMap::lastUsedChunk = 0;

CaveMap::CaveMap(logic::Logic &logic)
  : firstChunk(1)
{
  chunks[0].init({FixedPoint<0>{logic::SpawnPosX}.value / CHUNK_SIZE, 0});
  while (firstChunk)
    genNextChunk(logic);
}

void CaveMap::genNextChunk(logic::Logic &logic)
{
  CaveChunk &old(chunks[(chunks.size() + firstChunk - 1) % chunks.size()]);

  logic.removeAllEntitiesInChunk(old.getPos());
  chunks[firstChunk].init(old, logic);
  ++firstChunk %= chunks.size();
}

void CaveMap::regenIfNecessary(Vect<unsigned int, 2> position, logic::Logic &logic) noexcept
{
  Vect<unsigned int, 2> chunkPos(position / CHUNK_SIZE);

  for (size_t i(0u); i < chunks.size(); ++i)
    {
      CaveChunk &chunk(getChunk(i));
      if (!(chunk.getPos() - chunkPos).any())
	{
	  std::cout << "currentChunk" << i << std::endl;
	  while (i > 5)
	    {
	      logic.score += 100;
	      genNextChunk(logic);
	      --i;
	    }
	  return ;
	}
    }
  assert(!"This is unfortunate: you seem to be outside the generated level :/");
}

CaveChunk &CaveMap::getChunk(size_t index) noexcept
{
  return chunks[(firstChunk + index) % chunks.size()];
}

CaveChunk const &CaveMap::getChunk(size_t index) const noexcept
{
  return chunks[(firstChunk + index) % chunks.size()];
}

Tile &CaveMap::getTile(Vect<unsigned int, 2> position, size_t &lastUsedChunk) noexcept
{
  Vect<unsigned int, 2> chunkPos(position / CHUNK_SIZE);
  Vect<unsigned int, 2> relativePos(position % CHUNK_SIZE);

  {
    CaveChunk &chunk(chunks[lastUsedChunk]);
    if (!(chunk.getPos() - chunkPos).any())
      {
	return chunk.getTile(relativePos);
      }
  }
  for (size_t i(0u); i < chunks.size(); ++i)
    {
      CaveChunk &chunk(chunks[i]);
      if (!(chunk.getPos() - chunkPos).any())
	{
	  lastUsedChunk = i;
	  return chunk.getTile(relativePos);
	}
    }
  return outOfBounds;
}

Tile const &CaveMap::getTile(Vect<unsigned int, 2> position, size_t &lastUsedChunk) const noexcept
{
  return const_cast<CaveMap &>(*this).getTile(position, lastUsedChunk);
}
