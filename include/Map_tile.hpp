#pragma once

enum class tile_type : char
{
	None = 0,
	Wall,
	Entry,
	Platform
};

// various props
enum class decoration_type : char
{
	None = 0
};

class Cave_tile {
public:
  tile_type type;
  decoration_type decoration;

  Cave_tile(tile_type type = tile_type::Wall,
	    decoration_type decoration = decoration_type::None);

};
