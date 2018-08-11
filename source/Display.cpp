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

  void Display::render(logic::Logic const &logic)
  {
    window.clear();
    camera.offset = (Vect<double, 2u>(static_cast<double>(window.getSize().x), static_cast<double>(window.getSize().y)) / 2) - Vect<float, 2u>::fromFixedPoint<-8>(logic.getCameraPosition());
    for (logic::Entity const & entity : logic.getEntities())
      {
	sf::Sprite sprite(textures[entity.getTexture()]);
	Vect<float, 2u> position = camera.apply(Vect<float, 2u>::fromFixedPoint<-8>(entity.getPosition()));
	sprite.setOrigin(static_cast<float>(textures[entity.getTexture()].getSize().x / 2.0), static_cast<float>(textures[entity.getTexture()].getSize().y / 2.0));
	sprite.setPosition(position[0], position[1]);
	window.draw(sprite);
      }
    window.display();
  }

  sf::RenderWindow& Display::getWindow()
  {
    return (window);
  }
}
