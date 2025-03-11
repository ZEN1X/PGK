#include "hexagon.h"
#include <iostream>

hexagon::hexagon(sf::String name)
    : hexagon_outline{6}, slider_value{1.0f} {
  extern std::shared_ptr<sf::Font> font;

  color_space_text.setFont(*font);
  color_space_text.setCharacterSize(21);
  color_space_text.setFillColor(sf::Color::Black);
  color_space_text.setString(name);

  for (size_t i = 0; i < 3; ++i) {
    corner_labels[i].setFont(*font);
    corner_labels[i].setCharacterSize(21);
    corner_labels[i].setFillColor(sf::Color::Black);
    corner_labels[i].setString(name[i]);
  }

  border.setFillColor(sf::Color::Transparent);
  border.setOutlineColor(sf::Color::Black);
  border.setOutlineThickness(-1);

  hexagon_outline.setFillColor(sf::Color::Transparent);
  hexagon_outline.setOutlineColor(sf::Color::Black);
  hexagon_outline.setOutlineThickness(-2);

  texture.create(256, 256);
  sprite.setTexture(texture);
}

void hexagon::Set_Positions() {
  float padding{5.0f};

  border.setSize(sf::Vector2f(right_bottom.x - left_top.x - 2 * padding, right_bottom.y - left_top.y - 2 * padding));
  border.setPosition(left_top.x + padding, left_top.y + padding);

  color_space_text.setPosition(border.getPosition() + sf::Vector2f(padding / 2, padding / 2));

  for (size_t i = 0; i < 6; ++i) {
    hexagon_outline.setPoint(i, p[i]);
  }

  for (size_t i = 0; i < 3; ++i) {
    sf::FloatRect label_bounding_box = corner_labels[i].getLocalBounds();
    corner_labels[i].setOrigin(label_bounding_box.width / 2, label_bounding_box.height / 2);

    sf::Vector2f direction_vector(p[i * 2] - center);
    float direction_vector_length = std::sqrtf(std::powf(direction_vector.x, 2.0f) + std::powf(direction_vector.y, 2.0f));
    direction_vector /= direction_vector_length;
    corner_labels[i].setPosition(p[i * 2] + direction_vector * 15.0f);
  }

  // calculate the scale aka actual size / 256
  float actual_width = (p[5].x - p[1].x);
  float actual_height = (p[3].y - p[0].y);

  scale.x = actual_width / 256;
  scale.y = actual_height / 256;

  sprite.setScale(scale);
  sprite.setPosition(p[1].x, p[0].y);
  Update_Pixels();
  texture.update(pixels);
}

// Trochę matematyki jeszcze nikomu nie zaszkodziło. Więc dwie kolejne metody to czysta matematyka.
float hexagon::d_coefficient(sf::Vector2f p0, sf::Vector2f p1, sf::Vector2f p) const {
  float A = p0.y - p1.y;
  float B = p1.x - p0.x;
  float C = p0.x * (p1.y - p0.y) - p0.y * (p1.x - p0.x);

  return 2 * ((A * p.x + B * p.y + C) / sqrt(A * A + B * B)) / sqrt(3.0f);
}

void hexagon::Update_Texture() {
  Update_Pixels();
  texture.update(pixels);
}

void hexagon::update_slider_value(float new_slider_value) {
  slider_value = new_slider_value;
}

bool hexagon::rhombus(sf::Vector2f p0, sf::Vector2f p1, sf::Vector2f p, float& alpha, float& beta) const {
  float L = sqrt((p1.x - p0.x) * (p1.x - p0.x) + (p1.y - p0.y) * (p1.y - p0.y));
  sf::Vector2f p3(0.5f * (p0.x - p1.x) + 0.5f * sqrt(3.0f) * (p1.y - p0.y) + p0.x, 0.5f * sqrt(3.0f) * (p0.x - p1.x) - 0.5f * (p1.y - p0.y) + p0.y);

  beta = -d_coefficient(p0, p1, p) / L;
  alpha = d_coefficient(p0, p3, p) / L;

  if ((alpha > 1.0f) || (beta > 1.0f) || (alpha < 0.0f) || (beta < 0.0f))
    return false;
  else
    return true;
}

void hexagon::Set_Borders(sf::Vector2f _left_top, sf::Vector2f _right_bottom) {
  left_top = _left_top;
  right_bottom = _right_bottom;
  float a = ((right_bottom.y - left_top.y) / 2.0f + 0.5f) - 20.0f;
  center = left_top + sf::Vector2f((right_bottom.x - left_top.x) / 2.0f + 0.5f, (right_bottom.y - left_top.y) / 2.0f + 0.5f + 10);

  p[0] = center - sf::Vector2f(0.0f, a);
  p[1] = center - sf::Vector2f(0.5f * sqrt(3.0f) * a, 0.5f * a);
  p[2] = center - sf::Vector2f(0.5f * sqrt(3.0f) * a, -0.5f * a);
  p[3] = center + sf::Vector2f(0.0f, a);
  p[4] = center + sf::Vector2f(0.5f * sqrt(3.0f) * a, 0.5f * a);
  p[5] = center + sf::Vector2f(0.5f * sqrt(3.0f) * a, -0.5f * a);

  Set_Positions();
}

void hexagon::Draw_Border(sf::RenderTarget& target, sf::RenderStates states) const {
  // Tu trzeba narysować ramkę. I napisy.

  // draw the square'ish border
  target.draw(border, states);

  // draw the text with the color space name
  target.draw(color_space_text, states);

  // draw the hexagon outline
  target.draw(hexagon_outline, states);

  // corner labels
  for (size_t i = 0; i < 3; ++i) {
    target.draw(corner_labels[i], states);
  }
}
