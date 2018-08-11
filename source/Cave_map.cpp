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
