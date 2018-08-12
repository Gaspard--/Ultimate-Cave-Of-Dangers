#ifndef DISPLAY_HPP
# define DISPLAY_HPP

# include <SFML/Graphics.hpp>
# include <map>
# include <string>
# include <stdexcept>
# include <vector>

# include "logic/Logic.hpp"
# include "Camera.hpp"

namespace logic
{
  class Logic;
}

namespace disp
{

  enum class TextureList
    {
     LEGACY,
     CAVE_TILE,
     MINEKART,
     TABLE,
     BARREL,
     CORNER,
     SIDE,
     BOB
    };

  class	Display
  {

  public:

    Display();
    ~Display();

    bool isRunning() const;
    void render(logic::Logic const &);
    sf::RenderWindow& getWindow();

  private:

    void loadTexture(TextureList, std::string);
    void drawSprite(sf::Texture const&, Vect<float, 2u> position, Vect<float, 2u> size = {1.0f, 1.0f}) noexcept;

    sf::RenderWindow window;
    std::map<TextureList, sf::Texture> textures;
    Camera camera;

  };
}

#endif /* DISPLAY_HPP */
