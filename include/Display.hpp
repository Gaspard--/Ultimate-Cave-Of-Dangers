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
  class Entity;
}

class CaveMap;

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
     BOB,
     WALL,
     PIECE_OF_CORNER,
     PARALAX,
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
    void renderSprite(sf::Texture const&, Vect<float, 2u> position, float rotation = 0.0f, Vect<float, 2u> size = {1.0f, 1.0f}, Vect<int, 2u> repeat = {1, 1}) noexcept;
    void renderMap(CaveMap const &);
    void renderEntities(std::vector<logic::Entity>::const_iterator const &begin, std::vector<logic::Entity>::const_iterator const &end);
    void renderParalax(Vect<float, 2u> const &movement);

    Vect<float, 2u> paralaxPos;

    sf::RenderWindow window;
    std::map<TextureList, sf::Texture> textures;
    Camera camera;

  };
}

#endif /* DISPLAY_HPP */
