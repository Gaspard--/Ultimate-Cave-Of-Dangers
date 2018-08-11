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
    camera.zoom = {1 / 16.0f, 1 / 16.0f * float(window.getSize().x) / float(window.getSize().y)};
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

  void Display::setUpSprite(sf::Sprite& sprite, sf::Texture const &texture, Vect<float, 2u> position) noexcept
  {
    sprite.setTexture(texture);
    sprite.setOrigin(0, float(texture.getSize().y));
    position *= Vect<float, 2u>(float(window.getSize().x), -float(window.getSize().y));
    position += Vect<float, 2u>(float(window.getSize().x), float(window.getSize().y)) * 0.5f;
    sprite.setPosition(position[0], position[1]);
  }

  bool Display::isRunning() const
  {
    return (window.isOpen());
  }

  void Display::render(logic::Logic const &logic)
  {
    window.clear();
    const CaveMap& map = logic.getMap();
    Vect<unsigned, 2u> cameraPosition = Vect<unsigned, 2u>::fromFixedPoint<-8>(logic.getCameraPosition());
    camera.offset = cameraPosition;

    sf::Vector2u tileDim = textures.at(TextureList::CAVE_TILE).getSize();

    for (unsigned x = cameraPosition[0] - 16 ; x != cameraPosition[0] + 17 ; ++x)
      for (unsigned y = cameraPosition[1] - 16 ; y != cameraPosition[1] + 17 ; ++y)
	{
	  if (map.getTile(Vect<unsigned, 2u>(int(x), int(y))).type == TileType::Empty)
	    continue;
	  sf::Sprite sprite;
	  setUpSprite(sprite, textures[TextureList::CAVE_TILE], camera.apply(Vect<int, 2u>(x, y)));
	  sprite.setScale(camera.zoom[0] * window.getSize().x / tileDim.x,
			  camera.zoom[1] * window.getSize().y / tileDim.y);
	  window.draw(sprite);
	}

    for (logic::Entity const & entity : logic.getEntities())
      {
	sf::Sprite sprite;
	setUpSprite(sprite, textures[entity.getTexture()], camera.apply(Vect<float, 2u>::fromFixedPoint<-8>(entity.getPosition())));
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
