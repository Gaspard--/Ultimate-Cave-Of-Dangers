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
    camera.zoom = {1 / 64.0f, 1 / 64.0f * float(window.getSize().x) / float(window.getSize().y)};
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

  void Display::drawSprite(sf::Texture const &texture, Vect<float, 2u> position, Vect<float, 2u> size) noexcept
  {
    sf::Sprite sprite;
    sprite.setTexture(texture);
    sprite.setOrigin(0, float(texture.getSize().y));
    sprite.setScale(camera.zoom[0] * float(window.getSize().x) / float(texture.getSize().x) * size[0],
		    camera.zoom[1] * float(window.getSize().y) / float(texture.getSize().y) * size[1]);
    position *= Vect<float, 2u>(float(window.getSize().x), -float(window.getSize().y));
    position += Vect<float, 2u>(float(window.getSize().x), float(window.getSize().y)) * 0.5f;
    sprite.setPosition(position[0], position[1]);
    window.draw(sprite);
  }

  bool Display::isRunning() const
  {
    return (window.isOpen());
  }

  void Display::render(logic::Logic const &logic)
  {
    window.clear();
    const CaveMap& map = logic.getMap();
    camera.offset = Vect<float, 2u>::fromFixedPoint(logic.getCameraPosition());
    {
      unsigned const minX(unsigned(camera.offset[0] - 0.5f / camera.zoom[0]) - 1);
      unsigned const minY(unsigned(camera.offset[1] - 0.5f / camera.zoom[1]) - 1);
      unsigned const maxX(unsigned(camera.offset[0] + 0.5f / camera.zoom[0]) + 1);
      unsigned const maxY(unsigned(camera.offset[1] + 0.5f / camera.zoom[1]) + 1);

      for (unsigned x = minX ; x != maxX ; ++x)
	for (unsigned y = minY; y != maxY; ++y)
	  {
	    if (map.getTile(Vect<unsigned, 2u>(x, y)).type == TileType::Empty)
	      continue;
	    drawSprite(textures[TextureList::CAVE_TILE], camera.apply(Vect<int, 2u>(int(x), int(y))));
	  }
    }
    for (logic::Entity const & entity : logic.getEntities())
      drawSprite(textures[entity.getTexture()],
		 camera.apply(Vect<float, 2u>::fromFixedPoint(entity.getPosition())),
		 {entity.getSize()[0].getFloatValue(), entity.getSize()[1].getFloatValue()});
    window.display();
  }

  sf::RenderWindow& Display::getWindow()
  {
    return (window);
  }
}
