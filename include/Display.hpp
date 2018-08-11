#ifndef DISPLAY_HPP
# define DISPLAY_HPP

# include <SFML/Graphics.hpp>
# include <map>
# include <string>
# include <stdexcept>

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
    void render();
    sf::RenderWindow& getWindow();

  private:

    void loadTexture(TextureList, std::string);

    sf::RenderWindow window;
    std::map<TextureList, sf::Texture> textures;

  };
}

#endif /* DISPLAY_HPP */
