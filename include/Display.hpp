#ifndef DISPLAY_HPP
# define DISPLAY_HPP

# include <SFML/Graphics.hpp>
# include <map>
# include <string>
# include <stdexcept>
# include <vector>

# include "Vect.hpp"
# include "Camera.hpp"
# include "my_opengl.hpp"

namespace logic
{
  class Logic;
  class Entity;
  struct WaterDrop;
}

class CaveMap;

namespace disp
{

  enum class TextureList
    {
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
     ZOMBIE,
     WAVE,
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
    Vect<float, 2u> renderSprite(sf::Texture const&, Vect<float, 2u>, float = 0.0f, Vect<float, 2u> = {1.0f, 1.0f}, Vect<int, 2u> = {1, 1}, Vect<unsigned, 2u> = {1, 1}) noexcept;
    void renderMap(CaveMap const &);
    void renderWater(FixedPoint<-8> waterLevel);
    void renderEntities(std::vector<logic::Entity>::const_iterator const &begin, std::vector<logic::Entity>::const_iterator const &end);
    void renderParalax(Vect<float, 2u> const &movement);

    Vect<float, 2u> paralaxPos;

    sf::RenderWindow window;
    std::map<TextureList, sf::Texture> textures;
    Camera camera;

  };
}

#endif /* DISPLAY_HPP */
