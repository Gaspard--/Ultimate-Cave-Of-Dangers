# include "Display.hpp"

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
    camera.zoom = {1 / 64.0f, 1 / 64.0f * float(window.getSize().x) / float(window.getSize().y)};
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

  void Display::renderSprite(sf::Texture const &texture, Vect<float, 2u> position, float rotation, Vect<float, 2u> size) noexcept
  {
    sf::Sprite sprite;
    sprite.setTexture(texture);
    sprite.setOrigin(float(texture.getSize().x) / 2, float(texture.getSize().y) / 2);
    sprite.setRotation(rotation);
    sprite.setScale(camera.zoom[0] * float(window.getSize().x) / float(texture.getSize().x) * size[0],
		    camera.zoom[1] * float(window.getSize().y) / float(texture.getSize().y) * size[1]);
    position *= Vect<float, 2u>(float(window.getSize().x), -float(window.getSize().y));
    position += Vect<float, 2u>(float(window.getSize().x), float(window.getSize().y)) * 0.5f;
    sprite.setPosition(position[0], position[1]);
    window.draw(sprite);
  }

  void Display::renderMap(CaveMap const &map)
  {
    unsigned const minX(unsigned(camera.offset[0] - 0.5f / camera.zoom[0]) - 1);
    unsigned const minY(unsigned(camera.offset[1] - 0.5f / camera.zoom[1]) - 1);
    unsigned const maxX(unsigned(camera.offset[0] + 0.5f / camera.zoom[0]) + 2);
    unsigned const maxY(unsigned(camera.offset[1] + 0.5f / camera.zoom[1]) + 2);

    for (unsigned x = minX ; x != maxX ; ++x)
      for (unsigned y = minY; y != maxY; ++y)
	{
	  if (map.getTile(Vect<unsigned, 2u>(x, y)).type == TileType::Wall) {
	    float rotation = 0.0f;
	    TextureList textureType = TextureList::CAVE_TILE;
	    unsigned tileSum = (map.getTile(Vect<unsigned, 2u>(x - 1, y)).type != TileType::Wall)
	      + (map.getTile(Vect<unsigned, 2u>(x + 1, y)).type != TileType::Wall)
	      + (map.getTile(Vect<unsigned, 2u>(x, y - 1)).type != TileType::Wall)
	      + (map.getTile(Vect<unsigned, 2u>(x, y + 1)).type != TileType::Wall);
	    if (!tileSum) {
	      if (map.getTile(Vect<unsigned, 2u>(x + 1, y + 1)).type != TileType::Wall ||
		  map.getTile(Vect<unsigned, 2u>(x - 1, y - 1)).type != TileType::Wall ||
		  map.getTile(Vect<unsigned, 2u>(x - 1, y + 1)).type != TileType::Wall ||
		  map.getTile(Vect<unsigned, 2u>(x + 1, y - 1)).type != TileType::Wall) {
		textureType = TextureList::PIECE_OF_CORNER;
		rotation = (map.getTile(Vect<unsigned, 2u>(x + 1, y - 1)).type != TileType::Wall) * 180.0f
		  + (map.getTile(Vect<unsigned, 2u>(x - 1, y - 1)).type != TileType::Wall) * 270.0f
		  + (map.getTile(Vect<unsigned, 2u>(x + 1, y + 1)).type != TileType::Wall) * 90.0f;
	      }
	      else
		textureType = TextureList::WALL;
	    }
	    else if (tileSum == 1) {
	      textureType = TextureList::SIDE;
	      rotation = (map.getTile(Vect<unsigned, 2u>(x + 1, y)).type != TileType::Wall) * 180.0f
		+ (map.getTile(Vect<unsigned, 2u>(x, y - 1)).type != TileType::Wall) * 270.0f
		+ (map.getTile(Vect<unsigned, 2u>(x, y + 1)).type != TileType::Wall) * 90.0f;
	    } else {
	      textureType = TextureList::CORNER;
	      rotation = (float(map.getTile(Vect<unsigned, 2u>(x + 1, y)).type != TileType::Wall)
			  * float(map.getTile(Vect<unsigned, 2u>(x, y + 1)).type != TileType::Wall)) * 90.0f
		+ (float(map.getTile(Vect<unsigned, 2u>(x + 1, y)).type != TileType::Wall)
		   * float(map.getTile(Vect<unsigned, 2u>(x, y - 1)).type != TileType::Wall)) * 180.0f
		+ (float(map.getTile(Vect<unsigned, 2u>(x - 1, y)).type != TileType::Wall)
		   * float(map.getTile(Vect<unsigned, 2u>(x, y - 1)).type != TileType::Wall)) * 270.0f;
	    }
	    renderSprite(textures[textureType], camera.apply(Vect<int, 2u>(int(x), int(y))), rotation);
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

  void Display::render(logic::Logic const &logic)
  {
    window.clear();
    camera.offset = Vect<float, 2u>::fromFixedPoint(logic.getCameraPosition());
    renderMap(logic.getMap());
    renderEntities(logic.getEntities().begin(), logic.getEntities().end());
    window.display();
  }

  sf::RenderWindow& Display::getWindow()
  {
    return (window);
  }
}
