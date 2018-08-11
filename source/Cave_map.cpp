#include "Cave_map.hpp"

CaveMap::CaveMap()
  : firstChunk(1)
{
  chunks[0] = CaveChunk();
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

CaveTile &CaveMap::getTile(Vect<unsigned int, 2> position) noexcept
{
  Vect<unsigned int, 2> chunkPos(position / CHUNK_SIZE);
  Vect<unsigned int, 2> relativePos(position % CHUNK_SIZE);

  for (CaveChunk &chunk : chunks)
    {
      if (!(chunk.getPos() - chunkPos).any())
	{
	  return chunk.getTile(relativePos[0], relativePos[1]);
	}
    }
  return outOfBounds;
}

CaveTile const &CaveMap::getTile(Vect<unsigned int, 2> position) const noexcept
{
  return const_cast<CaveMap &>(*this).getTile(position);
}
