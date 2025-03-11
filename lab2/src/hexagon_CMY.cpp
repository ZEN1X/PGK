#include "hexagon_CMY.h"
#include <string>

void hexagon_CMY::cmy_to_rgb(float c, float m, float y, sf::Uint8& r, sf::Uint8& g, sf::Uint8& b) const {
  r = static_cast<sf::Uint8>((1 - c) * 255);
  g = static_cast<sf::Uint8>((1 - m) * 255);
  b = static_cast<sf::Uint8>((1 - y) * 255);
}

void hexagon_CMY::rgb_to_cmy(sf::Uint8 r, sf::Uint8 g, sf::Uint8 b, float& c, float& m, float& y) const {
  c = 1 - (r / 255.0f);
  m = 1 - (g / 255.0f);
  y = 1 - (b / 255.0f);
}

void hexagon_CMY::update_cmy_values(const sf::Vector2f& point) {
  sf::Vector2f relative(point.x - p[1].x, point.y - p[0].y);

  int pos = (static_cast<int>(relative.x) + static_cast<int>(relative.y) * 256) * 4;

  sf::Uint8 r = pixels[pos];
  sf::Uint8 g = pixels[pos + 1];
  sf::Uint8 b = pixels[pos + 2];

  float c, m, y;
  rgb_to_cmy(r, g, b, c, m, y);

  cmy_values.x = c;
  cmy_values.y = m;
  cmy_values.z = y;
}

hexagon_CMY::hexagon_CMY(sf::String name)
    : hexagon{name} {
  extern std::shared_ptr<sf::Font> font;

  cmy_values_text.setFont(*font);
  cmy_values_text.setCharacterSize(13);
  cmy_values_text.setFillColor(sf::Color::Red);
}

void hexagon_CMY::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  // Tu trzeba narysować sześciokąt CMY.
  target.draw(sprite, states);

  cmy_values_text.setPosition(p[0].x - 60.0f, p[0].y);
  std::string cmy_string = "(";
  cmy_string.append(std::to_string(cmy_values.x));
  cmy_string.append(",");
  cmy_string.append(std::to_string(cmy_values.y));
  cmy_string.append(",");
  cmy_string.append(std::to_string(cmy_values.z));
  cmy_string.append(")");
  cmy_values_text.setString(cmy_string);

  target.draw(cmy_values_text, states);

  Draw_Border(target, states);
}

void hexagon_CMY::Update_Pixels() {
  for (int y = 0; y < 256; ++y) {
    for (int x = 0; x < 256; ++x) {
      sf::Vector2f point(x * scale.x + p[1].x, y * scale.y + p[0].y);
      float alpha, beta;

      int pos = (x + y * 256) * 4;

      sf::Uint8 r, g, b;

      if (rhombus(p[0], p[1], point, alpha, beta)) {
        cmy_to_rgb(1.0f * slider_value, alpha * slider_value, beta * slider_value, r, g, b);
        pixels[pos + 0] = r;    // R
        pixels[pos + 1] = g;    // G
        pixels[pos + 2] = b;    // B
        pixels[pos + 3] = 255;  // A                                                               // A
      } else if (rhombus(p[2], p[3], point, alpha, beta)) {
        cmy_to_rgb(beta * slider_value, 1.0f * slider_value, alpha * slider_value, r, g, b);
        pixels[pos + 0] = r;    // R
        pixels[pos + 1] = g;    // G
        pixels[pos + 2] = b;    // B
        pixels[pos + 3] = 255;  // A                                                               // A
      } else if (rhombus(p[4], p[5], point, alpha, beta)) {
        cmy_to_rgb(alpha * slider_value, beta * slider_value, slider_value, r, g, b);
        pixels[pos + 0] = r;    // R
        pixels[pos + 1] = g;    // G
        pixels[pos + 2] = b;    // B
        pixels[pos + 3] = 255;  // A                                                               // A
      } else {
        // outside the hexagon, make it transparent
        pixels[pos + 3] = 0;  // A
      }
    }
  }
}

bool hexagon_CMY::contains(const sf::Vector2f& point) const {
  // float cube_width = p[5].x - p[1].x;
  // float cube_height = p[3].y  - p[0].y;

  return sprite.getGlobalBounds().contains(point);
}
