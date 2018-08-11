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
    camera.offset = cameraPosition;
    camera.zoom = {1 / 16.0f, 1 / 16.0f * float(window.getSize().x) / window.getSize().y};

    auto const convertToScreen([&](Vect<float, 2u> position) noexcept
    {
      position *= Vect<float, 2u>(float(window.getSize().x), -float(window.getSize().y));
      position += Vect<float, 2u>(float(window.getSize().x), float(window.getSize().y)) * 0.5f;
      return position;
    });

    const CaveMap& map = logic.getMap();
    sf::Vector2u tileDim = textures.at(TextureList::CAVE_TILE).getSize();
    // unsigned xDiff = static_cast<unsigned>((window.getSize().x / 2.0) / static_cast<double>(tileDim.x));
    // unsigned yDiff = static_cast<unsigned>((window.getSize().y / 2.0) / static_cast<double>(tileDim.y));

    for (unsigned x = cameraPosition[0] - 16 ; x != cameraPosition[0] + 17 ; ++x)
      for (unsigned y = cameraPosition[1] - 16 ; y != cameraPosition[1] + 17 ; ++y)
	{
	  if (map.getTile(Vect<unsigned, 2u>(int(x), int(y))).type == TileType::Empty)
	    continue;
	  sf::Sprite sprite(textures[TextureList::CAVE_TILE]);
	  Vect<float, 2u> position = convertToScreen(camera.apply(Vect<int, 2u>(x, y)));
      
	  sprite.setOrigin(0, tileDim.y);
	  sprite.setPosition(position[0], position[1]);
	  sprite.setScale(camera.zoom[0] * window.getSize().x / tileDim.x,
			  camera.zoom[1] * window.getSize().y / tileDim.y);
	  window.draw(sprite);
	}

    for (logic::Entity const & entity : logic.getEntities())
      {
	sf::Sprite sprite(textures[entity.getTexture()]);
	Vect<float, 2u> position = convertToScreen(camera.apply(Vect<float, 2u>::fromFixedPoint<-8>(entity.getPosition())));
	sprite.setOrigin(0, float(textures[entity.getTexture()].getSize().y));
	sprite.setPosition(position[0], position[1]);
	sprite.setScale(camera.zoom[0] * textures[entity.getTexture()].getSize().x * entity.getSize()[0].getFloatValue() / tileDim.x,
			camera.zoom[1] * textures[entity.getTexture()].getSize().y * entity.getSize()[1].getFloatValue() / tileDim.y);
	window.draw(sprite);
      }
    window.display();
  }

  sf::RenderWindow& Display::getWindow()
  {
    return (window);
  }
}
