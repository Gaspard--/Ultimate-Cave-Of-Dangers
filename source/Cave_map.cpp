#include "Cave_map.hpp"

Cave_map::Cave_map()
  : firstChunk(1)
{
  chunks[0] = Cave_chunk();
  while (firstChunk)
    genNextChunk();
}

void Cave_map::genNextChunk()
{
  chunks[firstChunk].init(chunks[(chunks.size() + firstChunk - 1) % chunks.size()]);
  ++firstChunk %= chunks.size();
}

Cave_chunk &Cave_map::getChunk(size_t index) noexcept
{
  return chunks[(firstChunk + index) % chunks.size()];
}

Cave_chunk const &Cave_map::getChunk(size_t index) const noexcept
{
  return chunks[(firstChunk + index) % chunks.size()];
}

Cave_tile &Cave_map::getTile(Vect<unsigned int, 2> position) noexcept
{
  Vect<unsigned int, 2> chunkPos(position / CHUNK_SIZE);
  Vect<unsigned int, 2> relativePos(position % CHUNK_SIZE);

  for (Cave_chunk &chunk : chunks)
    {
      if (!(chunk.get_pos() - chunkPos).any())
	{
	  return chunk.get_tile(relativePos[0], relativePos[1]);
	}
    }
  return outOfBounds;
}

Cave_tile const &Cave_map::getTile(Vect<unsigned int, 2> position) const noexcept
{
  return const_cast<Cave_map &>(*this).getTile(position);
}
