# include "Display.hpp"

namespace disp
{
  Display::Display():
    window(sf::VideoMode(1920, 1080), "Ultime Cave of Dangers")
  {

  }

  Display::~Display()
  {

  }

  bool Display::isRunning() const
  {
    return (window.isOpen());
  }

  void Display::render()
  {
    window.clear();
    window.display();
  }

  sf::RenderWindow& Display::getWindow()
  {
    return (window);
  }
}
