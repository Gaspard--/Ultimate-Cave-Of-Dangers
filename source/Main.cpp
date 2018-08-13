# include "Display.hpp"
# include "logic/Logic.hpp"
# include <iostream>
# include <SFML/Audio.hpp>

int main()
{
  try {
    sf::Music music;
    music.openFromFile("resources/LD42_stuck_in_a_cave.wav");
      music.setLoop(true);
    music.play();
      disp::Display display;
  logic::Logic logic;
  sf::RenderWindow& window = display.getWindow();

  sf::Clock clock;
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
      display.render(logic);
      float currentTime = clock.restart().asSeconds();
      float fps = 1.f / currentTime;
      std::cerr << "Current nbr of FPS : " << fps << std::endl;
    }
  } catch (std::exception& e) {
    std::cerr << "Error : " << e.what() << std::endl;
    return (1);
  }
  return (0);
}
