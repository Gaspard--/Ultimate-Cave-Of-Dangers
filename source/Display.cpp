#include "Display.hpp"
#include "logic/Logic.hpp"

namespace disp
{
  Display::Display():
    window(sf::VideoMode(1920, 1080), "Ultime Cave of Dangers")
  {
    /* load all textures here */
    loadTexture(TextureList::LEGACY, "./resources/planet.bmp");
    loadTexture(TextureList::CAVE_TILE, "./resources/cave_tile.png");
    loadTexture(TextureList::MINEKART, "./resources/minekart.png");
    loadTexture(TextureList::BARREL, "./resources/barrel.png");
    loadTexture(TextureList::CORNER, "./resources/corner.png");
    loadTexture(TextureList::SIDE, "./resources/side.png");
    loadTexture(TextureList::TABLE, "./resources/table.png");
    loadTexture(TextureList::BOB, "./resources/bob.png");
    loadTexture(TextureList::WALL, "./resources/wall.png");
    loadTexture(TextureList::PIECE_OF_CORNER, "./resources/piece_of_corner.png");
    loadTexture(TextureList::PARALAX, "./resources/back.png");
    textures[TextureList::WALL].setRepeated(true);
    window.setVerticalSyncEnabled(true);
  }

  Display::~Display()
  {

  }

  void Display::loadTexture(TextureList textureType, std::string path)
  {
    sf::Texture texture;

    if (!texture.loadFromFile(path))
      throw(std::runtime_error("File not found"));
    textures.emplace(textureType, texture);
  }

  bool Display::isRunning() const
  {
    return (window.isOpen());
  }

  void Display::renderSprite(sf::Texture const &texture, Vect<float, 2u> position, float rotation, Vect<float, 2u> size, Vect<int, 2u> repeat) noexcept
  {
    sf::Sprite sprite;

    sprite.setTexture(texture);
    sprite.setOrigin(float(texture.getSize().x) / 2, float(texture.getSize().y) / 2);
    if (rotation)
      sprite.setRotation(rotation);
    sprite.setScale(camera.zoom[0] * float(window.getSize().x) / float(texture.getSize().x) * size[0],
		    camera.zoom[1] * float(window.getSize().y) / float(texture.getSize().y) * size[1]);
    sprite.setTextureRect({0, 0, texture.getSize().x * repeat[0], texture.getSize().y * repeat[1]});
    position += Vect<float, 2u>(0.5, -0.5);
    position *= Vect<float, 2u>(float(window.getSize().x), -float(window.getSize().y));
    sprite.setPosition(position[0], position[1]);
    window.draw(sprite);
  }

  void Display::renderMap(CaveMap const &map)
  {
    unsigned const minX(unsigned(camera.offset[0] - 0.5f / camera.zoom[0]) - 1);
    unsigned const minY(unsigned(camera.offset[1] - 0.5f / camera.zoom[1]) - 1);
    unsigned const maxX(unsigned(camera.offset[0] + 0.5f / camera.zoom[0]) + 2);
    unsigned const maxY(unsigned(camera.offset[1] + 0.5f / camera.zoom[1]) + 2);
    std::vector<std::vector<TileType>> tiles;
    tiles.reserve(maxX - minX + 2);
    for (unsigned x = minX - 1; x != maxX + 1 ; ++x) {
      tiles.push_back(std::vector<TileType>());
      tiles.back().reserve(maxY - minY + 2);
      for (unsigned y = minY - 1; y != maxY + 1; ++y)
	tiles.back().push_back(map.getTile(Vect<unsigned, 2u>(x, y)).type);
    }

    for (unsigned x = 1 ; x != maxX - minX + 1; ++x)
      {
	unsigned wallCount = 0;
	for (unsigned y = 1 ; y != maxY - minY + 1; ++y)
	  if (tiles[x][y] == TileType::Wall) {
	    float rotation = 0.0f;
	    TextureList textureType = TextureList::CAVE_TILE;
	    unsigned tileSum = (tiles[x - 1][y] != TileType::Wall) + (tiles[x + 1][y] != TileType::Wall)
	      + (tiles[x][y - 1] != TileType::Wall) + (tiles[x][y + 1] != TileType::Wall);
	    if (!tileSum) {
	      if (tiles[x + 1][y + 1] != TileType::Wall ||
		  tiles[x - 1][y - 1] != TileType::Wall ||
		  tiles[x - 1][y + 1] != TileType::Wall ||
		  tiles[x + 1][y - 1] != TileType::Wall) {
		textureType = TextureList::PIECE_OF_CORNER;
		rotation = (tiles[x + 1][y - 1] != TileType::Wall) * 180.0f
		  + (tiles[x - 1][y - 1] != TileType::Wall) * 270.0f
		  + (tiles[x + 1][y + 1] != TileType::Wall) * 90.0f;
	      }
	      else
		textureType = TextureList::WALL;
	    }
	    else if (tileSum == 1) {
	      textureType = TextureList::SIDE;
	      rotation = (tiles[x + 1][y] != TileType::Wall) * 180.0f
		+ (tiles[x][y - 1] != TileType::Wall) * 270.0f
		+ (tiles[x][y + 1] != TileType::Wall) * 90.0f;
	    } else {
	      textureType = TextureList::CORNER;
	      rotation = (float(tiles[x + 1][y] != TileType::Wall) * float(tiles[x][y + 1] != TileType::Wall)) * 90.0f
		+ (float(tiles[x + 1][y] != TileType::Wall) * float(tiles[x][y - 1] != TileType::Wall)) * 180.0f
		+ (float(tiles[x - 1][y] != TileType::Wall) * float(tiles[x][y - 1] != TileType::Wall)) * 270.0f;
	    }
	    if (textureType == TextureList::WALL)
	      ++wallCount;
	    else
	      {
		if (wallCount)
		  {
		    renderSprite(textures[TextureList::WALL], camera.apply(Vect<int, 2u>(int(x + minX - 1), int(y + minY - 2))), 0.0f, {1.0f, 1.0f}, {1, wallCount});
		    wallCount = 0;
		  }
		renderSprite(textures[textureType], camera.apply(Vect<int, 2u>(int(x + minX - 1), int(y + minY - 1))), rotation);
	      }
	  } else {
	    if (wallCount)
	      {
		renderSprite(textures[TextureList::WALL], camera.apply(Vect<int, 2u>(int(x + minX - 1), int(y + minY - 2))), 0.0f, {1.0f, 1.0f}, {1, wallCount});
		wallCount = 0;
	      }
	  }
	if (wallCount)
	  {
	    renderSprite(textures[TextureList::WALL], camera.apply(Vect<int, 2u>(int(x + minX - 1), int(maxY - 1))), 0.0f, {1.0f, 1.0f}, {1, wallCount});
	    wallCount = 0;
	  }
      }
  }

  void Display::renderEntities(std::vector<logic::Entity>::const_iterator const &begin, std::vector<logic::Entity>::const_iterator const &end)
  {
    for (auto entity = begin ; entity != end ; ++entity)
      renderSprite(textures[entity->getTexture()],
		   camera.apply(Vect<float, 2u>::fromFixedPoint(entity->getPosition())), 0.0f,
		   {entity->getSize()[0].getFloatValue(), entity->getSize()[1].getFloatValue()});
  }

  void Display::renderParalax(Vect<float, 2u> const &movement)
  {
    sf::Texture& texture = textures[TextureList::PARALAX];
    paralaxPos += movement * Vect<float, 2u>(-2, 2);
    if (paralaxPos[0] < -1 * static_cast<float>(texture.getSize().x))
      paralaxPos[0] += texture.getSize().x;
    else if (paralaxPos[0] >= texture.getSize().x)
      paralaxPos[0] -= texture.getSize().x;
    if (paralaxPos[1] < -1 * static_cast<float>(texture.getSize().y))
      paralaxPos[0] += texture.getSize().y;
    else if (paralaxPos[1] >= texture.getSize().y)
      paralaxPos[0] -= texture.getSize().y;

    std::vector<Vect<float, 2u>> allParalaxPos;
    allParalaxPos.push_back(paralaxPos);
    if (paralaxPos[0] < 0)
      allParalaxPos.push_back({paralaxPos[0] + texture.getSize().x, paralaxPos[1]});
    if (paralaxPos[0] > 0)
      allParalaxPos.push_back({paralaxPos[0] - texture.getSize().x, paralaxPos[1]});
    if (paralaxPos[1] < 0)
      allParalaxPos.push_back({paralaxPos[0], paralaxPos[1] + texture.getSize().y});
    if (paralaxPos[1] > 0)
      allParalaxPos.push_back({paralaxPos[0], paralaxPos[1] - texture.getSize().y});
    if (paralaxPos[0] < 0 && paralaxPos[1] < 0)
      allParalaxPos.push_back({paralaxPos[0] + texture.getSize().x, paralaxPos[1] + texture.getSize().y});
    if (paralaxPos[0] < 0 && paralaxPos[1] > 0)
      allParalaxPos.push_back({paralaxPos[0] + texture.getSize().x, paralaxPos[1] - texture.getSize().y});
    if (paralaxPos[0] > 0 && paralaxPos[1] < 0)
      allParalaxPos.push_back({paralaxPos[0] - texture.getSize().x, paralaxPos[1] + texture.getSize().y});
    if (paralaxPos[0] > 0 && paralaxPos[1] > 0)
      allParalaxPos.push_back({paralaxPos[0] - texture.getSize().x, paralaxPos[1] - texture.getSize().y});
    for (auto i = allParalaxPos.begin() ; i != allParalaxPos.end() ; ++i)
      {
	sf::Sprite sprite(textures[TextureList::PARALAX]);
	sprite.setPosition((*i)[0], (*i)[1]);
	window.draw(sprite);
      }
  }

  void Display::renderWater(std::vector<logic::WaterDrop>::const_iterator const &begin, std::vector<logic::WaterDrop>::const_iterator const &end)
  {
    std::vector<float> data;

    data.reserve((end - begin) * 6 * 4);
    auto dest(std::back_inserter(data));
    for (auto it(begin); it < end; ++it)
      {
	for (Vect<float, 2> corner : std::array<Vect<float, 2>, 6>{{
	    {-1.0f, -1.0f},
	      {1.0f, -1.0f},
		{-1.0f, 1.0f},
		  {-1.0f, 1.0f},
		    {1.0f, -1.0f},
		      {1.0f, 1.0f}}})
	  {
	    auto position(camera.apply(Vect<float, 2u>{it->position[0].getFloatValue(), it->position[1].getFloatValue()}));
	    for (uint32_t dir(0u); dir != 2; ++dir)
	      ++dest = position[dir] - corner[dir] * logic::WaterDropSize.getFloatValue();
	    for (uint32_t dir(0u); dir != 2; ++dir)
	      ++dest = corner[dir];
	  }
      }
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);
    glDrawArrays(GL_TRIANGLES, 0, (end - begin) * 4);
  }

  void Display::render(logic::Logic const &logic)
  {
    //renderWater(logic.getWaterDrops().begin(), logic.getWaterDrops().end());
    window.clear();
    Vect<float, 2u> oldCameraPos = camera.offset;
    camera.offset = Vect<float, 2u>::fromFixedPoint(logic.getCameraPosition());
    camera.zoom = {1 / 64.0f, 1 / 64.0f * float(window.getSize().x) / float(window.getSize().y)};
    renderParalax(camera.offset - oldCameraPos);
    renderMap(logic.getMap());
    renderEntities(logic.getEntities().begin(), logic.getEntities().end());
    window.display();
  }

  sf::RenderWindow& Display::getWindow()
  {
    return (window);
  }
}
