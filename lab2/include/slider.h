#ifndef SLIDER_H
#define SLIDER_H

#include "SFML/Graphics.hpp"

class slider : public sf::Drawable {
 protected:
  sf::Vector2f left_top, right_bottom;
  sf::RectangleShape slider_bar;
  float value; // clamped between 0 and 1

 public:
  slider();
  void Set_Borders(sf::Vector2f _left_top, sf::Vector2f _right_bottom);
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
  bool contains(const sf::Vector2f& point) const; // ala sf::Rect::contains()
  float update_value(const sf::Vector2f& mouse_position);
};

#endif /* SLIDER_H */
