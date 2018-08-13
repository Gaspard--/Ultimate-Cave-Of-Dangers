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

  Vect<float, 2u> Display::renderSprite(sf::Texture const &texture, Vect<float, 2u> position, float rotation, Vect<float, 2u> size, Vect<int, 2u> repeat) noexcept
  {
    sf::Sprite sprite;

    sprite.setTexture(texture);
    sprite.setOrigin(float(texture.getSize().x) * 0.5f, float(texture.getSize().y) * 0.5f);
    if (rotation)
      sprite.setRotation(rotation);
    sprite.setScale(camera.zoom[0] * float(window.getSize().x) / float(texture.getSize().x) * size[0],
		    camera.zoom[1] * float(window.getSize().y) / float(texture.getSize().y) * size[1]);
    sprite.setTextureRect({0, 0, int(texture.getSize().x) * repeat[0], int(texture.getSize().y) * repeat[1]});
    position += Vect<float, 2u>(0.5f, -0.5f);
    position *= Vect<float, 2u>(float(window.getSize().x), -float(window.getSize().y));
    sprite.setPosition(position[0], position[1]);
    window.draw(sprite);
    return (position);
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

    auto renderWallGroup([this](int x, int y, unsigned int wallCount)
    {
      renderSprite(textures[TextureList::WALL], camera.apply(Vect<int, 2u>(x, y)), 0.0f, {1.0f, 1.0f}, {1, int(wallCount)});
    });

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
		    renderWallGroup(int(x + minX - 1), int(y + minY - 2), wallCount);
		    wallCount = 0;
		  }
		renderSprite(textures[textureType], camera.apply(Vect<int, 2u>(int(x + minX - 1), int(y + minY - 1))), rotation);
	      }
	  } else {
	    if (wallCount)
	      {
		renderWallGroup(int(x + minX - 1), int(y + minY - 2), wallCount);
		wallCount = 0;
	      }
	  }
	if (wallCount)
	  {
	    renderWallGroup(int(x + minX - 1), int(maxY - 1), wallCount);
	    wallCount = 0;
	  }
      }
  }

  void Display::renderEntities(std::vector<logic::Entity>::const_iterator const &begin, std::vector<logic::Entity>::const_iterator const &end)
  {
    for (auto entity = begin ; entity != end ; ++entity) {
      Vect<unsigned, 2u> const &hps = entity->getHps();
      Vect<float, 2u> position = renderSprite(textures[entity->getTexture()],
					      camera.apply(Vect<float, 2u>::fromFixedPoint(entity->getPosition())), 0.0f,
					      {entity->getSize()[0].getFloatValue(), entity->getSize()[1].getFloatValue()});
      if (hps[1]) {
	sf::RectangleShape hpBar({camera.zoom[0] * float(window.getSize().x) * entity->getSize()[0].getFloatValue(), 5.f});
	sf::RectangleShape damageBar({hpBar.getSize().x * float(hps[1] - hps[0]) / float(hps[1]), hpBar.getSize().y});
	damageBar.setFillColor(sf::Color(255, 0, 0));
	hpBar.setFillColor(sf::Color(0, 255, 0));
	hpBar.setOrigin(hpBar.getSize().x / 2, hpBar.getSize().y / 2);
	position[1] -= camera.zoom[1] * float(window.getSize().y) * entity->getSize()[1].getFloatValue() * 0.6f;
	hpBar.setPosition(position[0], position[1]);
	damageBar.setPosition(position[0] + hpBar.getSize().x - damageBar.getSize().x - hpBar.getSize().x / 2, position[1] - hpBar.getSize().y / 2);
	window.draw(hpBar);
	if (hps[0] != hps[1])
	  window.draw(damageBar);
      }
    }
  }

  void Display::renderParalax(Vect<float, 2u> const &movement)
  {
    sf::Texture& texture = textures[TextureList::PARALAX];
    paralaxPos += movement * Vect<float, 2u>(-2.0f, 2.0f);
    float textureWidth = static_cast<float>(texture.getSize().x);
    float textureHeight = static_cast<float>(texture.getSize().y);

    if (paralaxPos[0] < -1 * textureWidth)
      paralaxPos[0] += textureWidth;
    else if (paralaxPos[0] >= textureWidth)
      paralaxPos[0] -= textureWidth;
    if (paralaxPos[1] < -1 * textureHeight)
      paralaxPos[0] += textureHeight;
    else if (paralaxPos[1] >= textureHeight)
      paralaxPos[0] -= textureHeight;

    std::vector<Vect<float, 2u>> allParalaxPos;
    allParalaxPos.push_back(paralaxPos);
    if (paralaxPos[0] < 0)
      allParalaxPos.push_back({paralaxPos[0] + textureWidth, paralaxPos[1]});
    if (paralaxPos[0] > 0)
      allParalaxPos.push_back({paralaxPos[0] - textureWidth, paralaxPos[1]});
    if (paralaxPos[1] < 0)
      allParalaxPos.push_back({paralaxPos[0], paralaxPos[1] + textureHeight});
    if (paralaxPos[1] > 0)
      allParalaxPos.push_back({paralaxPos[0], paralaxPos[1] - textureHeight});
    if (paralaxPos[0] < 0 && paralaxPos[1] < 0)
      allParalaxPos.push_back({paralaxPos[0] + textureWidth, paralaxPos[1] + textureHeight});
    if (paralaxPos[0] < 0 && paralaxPos[1] > 0)
      allParalaxPos.push_back({paralaxPos[0] + textureWidth, paralaxPos[1] - textureHeight});
    if (paralaxPos[0] > 0 && paralaxPos[1] < 0)
      allParalaxPos.push_back({paralaxPos[0] - textureWidth, paralaxPos[1] + textureHeight});
    if (paralaxPos[0] > 0 && paralaxPos[1] > 0)
      allParalaxPos.push_back({paralaxPos[0] - textureWidth, paralaxPos[1] - textureHeight});
    for (auto i = allParalaxPos.begin() ; i != allParalaxPos.end() ; ++i)
      {
	sf::Sprite sprite(textures[TextureList::PARALAX]);
	sprite.setPosition((*i)[0], (*i)[1]);
	window.draw(sprite);
      }
  }

  void Display::renderWater(FixedPoint<-8> waterLevel)
  {
  }

  void Display::render(logic::Logic const &logic)
  {
    Vect<float, 2u> oldCameraPos = camera.offset;
    camera.offset = Vect<float, 2u>::fromFixedPoint(logic.getCameraPosition());
    camera.zoom = {1 / 64.0f, 1 / 64.0f * float(window.getSize().x) / float(window.getSize().y)};
 
    window.clear();
    renderWater(logic.getWaterLevel());
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
