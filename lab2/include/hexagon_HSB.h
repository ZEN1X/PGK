#ifndef HEXAGON_HSB_H
#define HEXAGON_HSB_H

#include "hexagon.h"

class hexagon_HSB : public hexagon {
 protected:
  void hsv_to_rgb(float h, float s, float v, sf::Uint8& r, sf::Uint8& g, sf::Uint8& b);

 public:
  hexagon_HSB(sf::String name);
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
  virtual void Update_Pixels();
};

#endif /* HEXAGON_HSB_H */
