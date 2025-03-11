#include "hexagon_HSB.h"

hexagon_HSB::hexagon_HSB(sf::String name)
    : hexagon{name} {}

void hexagon_HSB::hsv_to_rgb(float h, float s, float v, sf::Uint8& r, sf::Uint8& g, sf::Uint8& b) {
  if (s == 0.0f) {
    r = g = b = static_cast<sf::Uint8>(v * 255.0f);
  } else {
    float var_h = h * 6.0f;
    if (var_h == 6.0f)
      var_h = 0;

    int var_i = static_cast<int>(std::floor(var_h));
    float var_1 = v * (1.0f - s);
    float var_2 = v * (1.0f - s * (var_h - static_cast<float>(var_i)));
    float var_3 = v * (1.0f - s * (1.0f - (var_h - static_cast<float>(var_i))));

    float var_r, var_g, var_b;

    switch (var_i) {
      case 0:
        var_r = v;
        var_g = var_3;
        var_b = var_1;
        break;
      case 1:
        var_r = var_2;
        var_g = v;
        var_b = var_1;
        break;
      case 2:
        var_r = var_1;
        var_g = v;
        var_b = var_3;
        break;
      case 3:
        var_r = var_1;
        var_g = var_2;
        var_b = v;
        break;
      case 4:
        var_r = var_3;
        var_g = var_1;
        var_b = v;
        break;
      default:
        var_r = v;
        var_g = var_1;
        var_b = var_2;
        break;
    }

    r = static_cast<sf::Uint8>(var_r * 255.0f);
    g = static_cast<sf::Uint8>(var_g * 255.0f);
    b = static_cast<sf::Uint8>(var_b * 255.0f);
  }
}

void hexagon_HSB::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  // Tu trzeba narysować sześciokąt HSB.
  target.draw(sprite, states);

  Draw_Border(target, states);
}

void hexagon_HSB::Update_Pixels() {
  for (int y = 0; y < 256; ++y) {
    for (int x = 0; x < 256; ++x) {
      sf::Vector2f point(x * scale.x + p[1].x, y * scale.y + p[0].y);
      float alpha, beta;

      int pos = (x + y * 256) * 4;

      float v = slider_value;
      sf::Uint8 r, g, b;

      if (rhombus(p[0], p[1], point, alpha, beta)) {
        hsv_to_rgb(1.0f, alpha, beta * v, r, g, b);
        pixels[pos + 0] = r;    // R
        pixels[pos + 1] = g;    // G
        pixels[pos + 2] = b;    // B
        pixels[pos + 3] = 255;  // A
      } else if (rhombus(p[2], p[3], point, alpha, beta)) {
        hsv_to_rgb(beta, 1.0f, alpha * v, r, g, b);
        pixels[pos + 0] = r;    // R
        pixels[pos + 1] = g;    // G
        pixels[pos + 2] = b;    // B
        pixels[pos + 3] = 255;  // A
      } else if (rhombus(p[4], p[5], point, alpha, beta)) {
        hsv_to_rgb(alpha, beta, v, r, g, b);
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
