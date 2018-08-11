# include "Input.hpp"

namespace in
{

  Input::Input(sf::RenderWindow& window):
    window(window)
  {

  }

  Input::~Input()
  {

  }

  bool Input::pollEvent(sf::Event& e)
  {
    return (window.pollEvent(e));
  }

}
