#pragma once

enum class TileType : char
  {
    Wall,
    Empty,
    Entry,
    Platform
  };

// various props
enum class DecorationType : char
  {
    None = 0
  };

class Tile {
public:
  TileType type;
  DecorationType decoration;

  Tile(TileType type = TileType::Wall,
       DecorationType decoration = DecorationType::None);

};
