#pragma once

enum class TileType : char
  {
    Wall,
    Empty,
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

  constexpr bool isSolid() const noexcept
  {
    return type == TileType::Wall;
  }

  constexpr bool isSolidAbove() const noexcept
  {
    return isSolid() || type == TileType::Platform;
  }
};
