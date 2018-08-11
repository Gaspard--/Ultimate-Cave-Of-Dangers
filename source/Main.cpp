# include "Display.hpp"
# include "logic/Logic.hpp"
# include <iostream>

int main()
{
  try {
      disp::Display display;
  logic::Logic logic;
  sf::RenderWindow& window = display.getWindow();

  while (display.isRunning())
    {
      sf::Event e;
      while (window.pollEvent(e)) {
	if (e.type == sf::Event::Closed)
	  window.close();
	else
	  logic.handleEvent(e);
      }
      logic.update();
      display.render();
    }
  } catch (std::exception& e) {
    std::cerr << "Error : " << e.what() << std::endl;
    return (1);
  }
  return (0);
}
