#ifndef HEXAGON_HSL_H
#define HEXAGON_HSL_H

#include "hexagon.h"

class hexagon_HSL : public hexagon {
 protected:
  void hsl_to_rgb(float h, float s, float l, sf::Uint8& r, sf::Uint8& g, sf::Uint8& b);
  sf::Uint8 Hue_2_RGB(float v1, float v2, float vH);

 public:
  hexagon_HSL(sf::String name);
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
  virtual void Update_Pixels();
};

#endif /* HEXAGON_HSL_H */
