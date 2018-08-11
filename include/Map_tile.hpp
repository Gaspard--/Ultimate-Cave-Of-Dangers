#pragma once

enum class TileType : char
{
	None = 0,
	Wall,
	Entry,
	Platform
};

// various props
enum class DecorationType : char
{
	None = 0
};

class CaveTile {
public:
  TileType type;
  DecorationType decoration;

  CaveTile(TileType type = TileType::Wall,
	   DecorationType decoration = DecorationType::None);

};
