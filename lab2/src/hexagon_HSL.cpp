#include "hexagon_HSL.h"

void hexagon_HSL::hsl_to_rgb(float h, float s, float l, sf::Uint8& r, sf::Uint8& g, sf::Uint8& b) {
  if (s == 0) {
    r = g = b = static_cast<sf::Uint8>(l * 255);
  } else {
    float var1, var2;

    if (l < 0.5f) {
      var2 = l * (1 + s);
    } else {
      var2 = (l + s) - (s * l);
    }

    var1 = 2 * l - var2;

    r = Hue_2_RGB(var1, var2, h + (1.0f / 3));
    g = Hue_2_RGB(var1, var2, h);
    b = Hue_2_RGB(var1, var2, h - (1.0f / 3));
  }
}

sf::Uint8 hexagon_HSL::Hue_2_RGB(float v1, float v2, float vH) {
  if (vH < 0)
    vH += 1;
  if (vH > 1)
    vH -= 1;
  if ((6 * vH) < 1)
    return static_cast<sf::Uint8>((v1 + (v2 - v1) * 6 * vH) * 255);
  if ((2 * vH) < 1)
    return static_cast<sf::Uint8>(v2 * 255);
  if ((3 * vH) < 2)
    return static_cast<sf::Uint8>((v1 + (v2 - v1) * ((2.0f / 3) - vH) * 6) * 255);

  return static_cast<sf::Uint8>(v1 * 255);
}

hexagon_HSL::hexagon_HSL(sf::String name)
    : hexagon{name} {}

void hexagon_HSL::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  // Tu trzeba narysować sześciokąt HSL.
  target.draw(sprite, states);

  Draw_Border(target, states);
}

void hexagon_HSL::Update_Pixels() {
  for (int y = 0; y < 256; ++y) {
    for (int x = 0; x < 256; ++x) {
      sf::Vector2f point(x * scale.x + p[1].x, y * scale.y + p[0].y);
      float alpha, beta;

      int pos = (x + y * 256) * 4;

      float l = slider_value;
      sf::Uint8 r, g, b;

      if (rhombus(p[0], p[1], point, alpha, beta)) {
        hsl_to_rgb(1.0f, alpha, beta * l, r, g, b);
        pixels[pos + 0] = r;    // R
        pixels[pos + 1] = g;    // G
        pixels[pos + 2] = b;    // B
        pixels[pos + 3] = 255;  // A
      } else if (rhombus(p[2], p[3], point, alpha, beta)) {
        hsl_to_rgb(beta, 1.0f, alpha * l, r, g, b);
        pixels[pos + 0] = r;    // R
        pixels[pos + 1] = g;    // G
        pixels[pos + 2] = b;    // B
        pixels[pos + 3] = 255;  // A
      } else if (rhombus(p[4], p[5], point, alpha, beta)) {
        hsl_to_rgb(alpha, beta, l, r, g, b);
        pixels[pos + 0] = r;    // R
        pixels[pos + 1] = g;    // G
        pixels[pos + 2] = b;    // B
        pixels[pos + 3] = 255;  // A
      } else {
        // outside the hexagon, make it transparent
        pixels[pos + 3] = 0;  // A
      }
    }
  }
}
