# include "logic/Anim.hpp"
# include <iostream>

namespace logic
{
  Anim::Anim(Vect<FixedPoint<-8>, 2u> const& position, float speed, disp::TextureList texture, unsigned nbFrames)
    : position(position)
    , speed(speed)
    , texture(texture)
    , frame(0)
    , nbFrames(nbFrames)
  {}

  Anim::~Anim(void) {}

  bool Anim::shouldBeRemoved() const noexcept
  {
    return (frame >= nbFrames);
  }

  Vect<unsigned, 2u> Anim::getAnimationPosition() const
  {
    return (Vect<unsigned, 2u>(frame, nbFrames));
  }

  void Anim::update()
  {
    if (timer.getElapsedTime().asSeconds() >= 1.f / speed) {
      timer.restart();
      ++frame;
    }
  }

  disp::TextureList const & Anim::getTexture() const
  {
    return (texture);
  }

  Vect<FixedPoint<-8>, 2u> const & Anim::getPosition() const
  {
    return (position);
  }

}
