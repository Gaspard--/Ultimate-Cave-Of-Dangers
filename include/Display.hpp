#ifndef DISPLAY_HPP
# define DISPLAY_HPP

# include <SFML/Graphics.hpp>
# include <map>
# include <string>
# include <stdexcept>
# include <vector>

# include "logic/Logic.hpp"

namespace logic
{
  class Logic;
}

namespace disp
{

  enum class TextureList
    {
     LEGACY
    };

  class	Display
  {

  public:

    Display();
    ~Display();

    bool isRunning() const;
    void render(logic::Logic&);
    sf::RenderWindow& getWindow();

  private:

    void loadTexture(TextureList, std::string);

    sf::RenderWindow window;
    std::map<TextureList, sf::Texture> textures;

  };
}

#endif /* DISPLAY_HPP */
