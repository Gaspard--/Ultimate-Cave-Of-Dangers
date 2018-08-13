#ifndef ANIM_HPP
# define ANIM_HPP

# include "Vect.hpp"
# include "FixedPoint.hpp"
# include "Display.hpp"
# include <SFML/Graphics.hpp>

namespace disp
{
  enum class TextureList;
}

namespace logic
{
  class	Anim
  {

  public:

    Anim(Vect<FixedPoint<-8>, 2u> const& position, float speed, disp::TextureList, unsigned nbFrames = 5);
    ~Anim(void);

    bool shouldBeRemoved() const noexcept;
    void update();
    Vect<unsigned, 2u> getAnimationPosition() const;
    disp::TextureList const & getTexture() const;
    Vect<FixedPoint<-8>, 2u> const & getPosition() const;

  private:

    disp::TextureList texture;
    Vect<FixedPoint<-8>, 2u> position;
    sf::Clock timer;
    float speed;
    unsigned frame;
    unsigned nbFrames;

  };

}

#endif /* ANIM_HPP */
