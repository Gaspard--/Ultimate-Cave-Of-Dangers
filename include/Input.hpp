#ifndef INPUT_HPP
# define INPUT_HPP

# include <SFML/Graphics.hpp>

namespace in
{
  class	Input
  {

  public:

    Input(sf::RenderWindow& window);
    ~Input();

    bool pollEvent(sf::Event& e);

  private:

    sf::RenderWindow& window;

  };
}

#endif /* INPUT_HPP */
