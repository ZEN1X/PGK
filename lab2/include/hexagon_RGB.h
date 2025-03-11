#ifndef HEXAGON_RGB_H
#define HEXAGON_RGB_H

#include "hexagon.h"

class hexagon_RGB : public hexagon {
 public:
  hexagon_RGB(sf::String name);
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
  virtual void Update_Pixels();
};

#endif /* HEXAGON_RGB_H */
