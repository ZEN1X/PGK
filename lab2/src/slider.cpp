#include "slider.h"

slider::slider()
    : slider_bar{}, value{1.0f} {
  slider_bar.setFillColor(sf::Color::Transparent);
  slider_bar.setOutlineColor(sf::Color::Black);
  slider_bar.setOutlineThickness(1);
}

void slider::Set_Borders(sf::Vector2f _left_top, sf::Vector2f _right_bottom) {
  left_top = _left_top;
  right_bottom = _right_bottom;

  slider_bar.setSize({right_bottom.x - left_top.x, right_bottom.y - left_top.y});
  slider_bar.setPosition(left_top);
}

void slider::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  // gradient quad
  sf::VertexArray gradient(sf::Quads, 4);

  // map the quad to the correct positions
  gradient[0].position = sf::Vector2f(left_top.x, left_top.y);
  gradient[1].position = sf::Vector2f(right_bottom.x, left_top.y);
  gradient[2].position = sf::Vector2f(right_bottom.x, right_bottom.y);
  gradient[3].position = sf::Vector2f(left_top.x, right_bottom.y);

  // set the colors to form the gradient
  gradient[0].color = sf::Color::Black;  // Top-left corner
  gradient[1].color = sf::Color::Black;  // Top-right corner
  gradient[2].color = sf::Color::White;  // Bottom-right corner
  gradient[3].color = sf::Color::White;  // Bottom-left corner

  // draw the gradient
  target.draw(gradient, states);

  // draw the slider bar (the slider itself)
  target.draw(slider_bar, states);

  // slider handle
  float width = right_bottom.x - left_top.x;
  float height = 20.0f;
  sf::RectangleShape slider_handle({width, height});
  slider_handle.setFillColor(sf::Color::Red);
  slider_handle.setOutlineColor(sf::Color::Black);
  slider_handle.setOutlineThickness(2);
  slider_handle.setOrigin(width / 2, 10.f);
  slider_handle.setPosition(left_top.x + width/2, value * (right_bottom.y - left_top.y) + left_top.y);

  // draw the handle (the thingy you hold)
  target.draw(slider_handle, states);
}

bool slider::contains(const sf::Vector2f& point) const {
  return slider_bar.getGlobalBounds().contains(point);
}

float slider::update_value(const sf::Vector2f& mouse_position) {
  sf::Vector2f relative_pos = mouse_position - slider_bar.getPosition();
  float scaled = relative_pos.y / slider_bar.getSize().y;
  if (scaled < 0) {
    scaled = 0;
  } else if (scaled > 1) {
    scaled = 1;
  }
  value = scaled;

  return value;
}
