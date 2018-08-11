# include "Display.hpp"

namespace disp
{
  Display::Display():
    window(sf::VideoMode(1920, 1080), "Ultime Cave of Dangers")
  {
    /* load all textures here */
    loadTexture(TextureList::LEGACY, "./resources/planet.bmp");
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

  void Display::render(logic::Logic& logic)
  {
    window.clear();
    for (logic::Entity const & entity : logic.getEntities())
      {
	sf::Sprite sprite(textures[entity.getTexture()]);
	window.draw(sprite);
      }
    window.display();
  }

  sf::RenderWindow& Display::getWindow()
  {
    return (window);
  }
}
