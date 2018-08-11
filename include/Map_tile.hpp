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
	decoration decoration;
};
