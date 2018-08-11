# include "Display.hpp"

int main()
{
  disp::Display display;
  sf::RenderWindow& window = display.getWindow();

  while (display.isRunning())
    {
      sf::Event e;
      while (window.pollEvent(e)) {
	if (e.type == sf::Event::Closed)
	  window.close();
      }
      display.render();
    }
  return (0);
}
