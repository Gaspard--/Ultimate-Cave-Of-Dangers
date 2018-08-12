#include "Cave_map.hpp"

size_t CaveMap::lastUsedChunk = 0;

CaveMap::CaveMap()
  : firstChunk(1)
{
  chunks[0].init({0, 0});
  while (firstChunk)
    genNextChunk();
}

void CaveMap::genNextChunk()
{
  chunks[firstChunk].init(chunks[(chunks.size() + firstChunk - 1) % chunks.size()]);
  ++firstChunk %= chunks.size();
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
