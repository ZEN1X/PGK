#ifndef HEXAGON_CMY_H
#define HEXAGON_CMY_H

#include "hexagon.h"

class hexagon_CMY : public hexagon {
  protected:
  void cmy_to_rgb(float c, float m, float y, sf::Uint8& r, sf::Uint8& g, sf::Uint8& b) const;
  void rgb_to_cmy(sf::Uint8 r, sf::Uint8 g, sf::Uint8 b, float& c, float& m, float& y) const;
  mutable sf::Text cmy_values_text; // i guess thats the only option :(
  sf::Vector3f cmy_values;
  

 public:
  hexagon_CMY(sf::String name);
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
  virtual void Update_Pixels();
  bool contains(const sf::Vector2f& point) const;
  void update_cmy_values(const sf::Vector2f& point);
};

#endif /* HEXAGON_CMY_H */
