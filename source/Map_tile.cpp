#include "Map_tile.h"

Cave_tile::Cave_tile (	tile_type type = tile_type::Wall,
			decoration_type decoration = decoration_type::None)
{
	Cave_tile.type = type;
	Cave_tile.decoration = decoration;
}
