# include "Display.hpp"
# include "Input.hpp"

int main()
{
  disp::Display display;
  in::Input input(display.getWindow());

  while (display.isRunning())
    {
      sf::Event e;
      while (input.pollEvent(e)) {
	if (e.type == sf::Event::Closed)
	  display.getWindow().close();
      }
      display.render();
    }
  return (0);
}
