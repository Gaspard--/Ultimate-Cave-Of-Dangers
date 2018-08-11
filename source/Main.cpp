# include "Display.hpp"
# include "logic/Logic.hpp"

int main()
{
  disp::Display display;
  logic::Logic logic;
  sf::RenderWindow& window = display.getWindow();

  while (display.isRunning())
    {
      sf::Event e;
      while (window.pollEvent(e)) {
	if (e.type == sf::Event::Closed)
	  window.close();
      }
      logic.update();
      display.render();
    }
  return (0);
}
