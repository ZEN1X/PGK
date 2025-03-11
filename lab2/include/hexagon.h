#ifndef HEXAGON_H
#define HEXAGON_H

#include <SFML/Graphics.hpp>

class hexagon : public sf::Drawable {
 protected:
  sf::Vector2f left_top, right_bottom, center;
  sf::Vector2f p[6];  // Kolejność punktów opisana w pliku PDF do laboratorium.
  sf::Text color_space_text;
  std::array<sf::Text, 3> corner_labels;
  sf::RectangleShape border;
  sf::ConvexShape hexagon_outline;
  sf::Texture texture;
  sf::Uint8 pixels[256 * 256 * 4];
  sf::Vector2f scale;
  sf::Sprite sprite;
  float slider_value; // clamped to (0, 1)


 public:
  hexagon(sf::String name);
  void Set_Positions();
  void Set_Borders(sf::Vector2f _left_top, sf::Vector2f _right_bottom);
  void Draw_Border(sf::RenderTarget& target, sf::RenderStates states) const;
  bool rhombus(sf::Vector2f p0, sf::Vector2f p1, sf::Vector2f p, float& alpha, float& beta) const;
  virtual void Update_Pixels() = 0;
  void Update_Texture();
  void update_slider_value(float new_slider_value);

 private:
  float d_coefficient(sf::Vector2f p0, sf::Vector2f p1, sf::Vector2f p) const;
};

#endif /* HEXAGON_H */
