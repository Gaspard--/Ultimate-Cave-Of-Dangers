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

  void Display::render(logic::Logic const &logic)
  {
    window.clear();
    Vect<unsigned, 2u> cameraPosition = Vect<unsigned, 2u>::fromFixedPoint<-8>(logic.getCameraPosition());
    camera.offset = (Vect<double, 2u>(static_cast<double>(window.getSize().x), static_cast<double>(window.getSize().y)) / 2.0) - cameraPosition;
    const CaveMap& map = logic.getMap();
    sf::Vector2u tileDim = textures.at(TextureList::CAVE_TILE).getSize();
    unsigned xDiff = static_cast<unsigned>((window.getSize().x / 2.0) / static_cast<double>(tileDim.x));
    unsigned yDiff = static_cast<unsigned>((window.getSize().y / 2.0) / static_cast<double>(tileDim.y));

    for (unsigned x = cameraPosition[0] - xDiff ; x < cameraPosition[0] + xDiff ; ++x)
      for (unsigned y = cameraPosition[1] - yDiff ; y < cameraPosition[1] + yDiff ; ++y)
	{
	  if (map.getTile(Vect<unsigned, 2u>(x, y)).type == TileType::None)
	    continue;
	  sf::Sprite sprite(textures[TextureList::CAVE_TILE]);
	  Vect<float, 2u> position = camera.apply(Vect<unsigned, 2u>(x, y));
	  sprite.setPosition(position[0], static_cast<float>(window.getSize().y) - position[1]);
	  window.draw(sprite);
	}

    for (logic::Entity const & entity : logic.getEntities())
      {
	sf::Sprite sprite(textures[entity.getTexture()]);
	Vect<float, 2u> position = camera.apply(Vect<float, 2u>::fromFixedPoint<-8>(entity.getPosition()));
	sprite.setPosition(position[0], static_cast<float>(window.getSize().y) - position[1]);
	window.draw(sprite);
      }
    window.display();
  }

  sf::RenderWindow& Display::getWindow()
  {
    return (window);
  }
}
