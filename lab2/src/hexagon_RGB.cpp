#include "hexagon_RGB.h"

hexagon_RGB::hexagon_RGB(sf::String name)
    : hexagon{name} {}

void hexagon_RGB::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  // Tu trzeba narysować sześciokąt RGB.
  target.draw(sprite, states);

  Draw_Border(target, states);
}

void hexagon_RGB::Update_Pixels() {
  for (int y = 0; y < 256; ++y) {
    for (int x = 0; x < 256; ++x) {
      sf::Vector2f point(x * scale.x + p[1].x, y * scale.y + p[0].y);
      float alpha, beta;

      int pos = (x + y * 256) * 4;

      if (rhombus(p[0], p[1], point, alpha, beta)) {
        pixels[pos + 0] = 255 * slider_value;                                  // R
        pixels[pos + 1] = static_cast<sf::Uint8>(alpha * 255 * slider_value);  // G
        pixels[pos + 2] = static_cast<sf::Uint8>(beta * 255 * slider_value);   // B
        pixels[pos + 3] = 255;                                                 // A
      } else if (rhombus(p[2], p[3], point, alpha, beta)) {
        pixels[pos + 0] = static_cast<sf::Uint8>(beta * 255 * slider_value);   // R
        pixels[pos + 1] = 255 * slider_value;                                  // G
        pixels[pos + 2] = static_cast<sf::Uint8>(alpha * 255 * slider_value);  // B
        pixels[pos + 3] = 255;                                                 // A
      } else if (rhombus(p[4], p[5], point, alpha, beta)) {
        pixels[pos + 0] = static_cast<sf::Uint8>(alpha * 255 * slider_value);  // R
        pixels[pos + 1] = static_cast<sf::Uint8>(beta * 255 * slider_value);   // G
        pixels[pos + 2] = 255 * slider_value;                                  // B
        pixels[pos + 3] = 255;                                                 // A
      } else {
        // outside the hexagon, make it transparent
        pixels[pos + 3] = 0;  // A
      }
    }
  }
}
