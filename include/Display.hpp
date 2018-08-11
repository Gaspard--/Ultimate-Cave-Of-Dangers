#ifndef DISPLAY_HPP
# define DISPLAY_HPP

# include <SFML/Graphics.hpp>

namespace disp
{
  class	Display
  {

  public:

    Display();
    ~Display();

    bool isRunning() const;
    void render();
    sf::RenderWindow& getWindow();

  private:

    sf::RenderWindow window;

  };
}

#endif /* DISPLAY_HPP */
