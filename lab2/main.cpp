// Uwaga! Co najmniej C++17!!!
// Project-> ... Properties->Configuration Properties->General->C++ Language Standard == ISO C++ 17 Standard (/std:c++17)

#include <iostream>
#include <numeric>
#include "SFML/Graphics.hpp"
#include "ShareTechMono-Regular.h"
#include "hexagon_CMY.h"
#include "hexagon_HSB.h"
#include "hexagon_HSL.h"
#include "hexagon_RGB.h"
#include "slider.h"

// Tak – dobrze państwo widzą – TO jest zmienna globalna! Czytanie i przetwarzanie fontów w SFML jest bardzo kosztowne. Dlatego zrobimy to raz.
// Co nie zmienia faktu, że można by to zrobić bez zmiennej globalnej i to całkiem ładnie. Jak? To już dla Państwa :-)
std::shared_ptr<sf::Font> font;

int main() {
  sf::RenderWindow window(sf::VideoMode(800, 600), "GFK Lab 02", sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize);
  // window.setFramerateLimit(60);

  sf::Clock frame_clock, around_half_secound_clock;
  sf::Int64 mean_frames_time = 0;
  std::vector<sf::Int64> frame_times;
  sf::Text text;

  font = std::make_shared<sf::Font>();
  font->loadFromMemory(&(ShareTechMono_Regular_ttf[0]), ShareTechMono_Regular_ttf.size());
  text.setFont(*font);
  text.setCharacterSize(21);
  text.setFillColor(sf::Color::Black);

  hexagon_RGB h_RGB("RGB");
  hexagon_CMY h_CMY("CMY");
  hexagon_HSL h_HSL("HSL");
  hexagon_HSB h_HSB("HSB");
  slider shade_slider;

  bool is_slider_dragged = false;
  float slider_val = 1.0f;
  sf::Vector2f mouse_pos;

  // Nie mogę uniknąć tych "magic numbers" bo bym zdradził w ten sposób co ma być w sf::Event::Resize. A tego byśmy nie chcieli - prawda? :-D
  text.setPosition(30, 30);
  h_RGB.Set_Borders(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(300.0f, 294.0f));
  h_CMY.Set_Borders(sf::Vector2f(300.0f, 0.0f), sf::Vector2f(600.0f, 294.0f));
  h_HSL.Set_Borders(sf::Vector2f(0.0f, 294.0f), sf::Vector2f(300.0f, 588.0f));
  h_HSB.Set_Borders(sf::Vector2f(300.0f, 294.0f), sf::Vector2f(600.0f, 588.0f));
  shade_slider.Set_Borders(sf::Vector2f(620.0f, 10.0f), sf::Vector2f(780.0f, 588.0f));

  h_RGB.update_slider_value(0.5);

  around_half_secound_clock.restart();

  float slider_space = 212.0f; // DO NOT TOUCH!! (somehow it works)

  while (window.isOpen()) {
    mouse_pos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    sf::Event event;
    window.clear(sf::Color::White);

    frame_clock.restart();  // Start pomiaru czasu.

    // Tu trzeba obsłużyć zdarzenia: zmianę rozmiaru okna oraz klikanie w „suwaczek”.
    while (window.pollEvent(event)) {
      switch (event.type) {
        case sf::Event::Closed:
          window.close();
          break;

        case sf::Event::Resized: {
          unsigned int window_width = event.size.width;
          unsigned int window_height = event.size.height;

          // dont let the window get too small
          if (window_width < 800 || window_height < 600) {
            window.setView(sf::View(sf::FloatRect(0, 0, 800, 600)));
            window.setSize(sf::Vector2u(800, 600));
          } else {
            // this is to leave enough space for the slider
            float borderSize = window_height > (window_width - slider_space) ? (window_width - slider_space) / 2 : window_height / 2;
            h_RGB.Set_Borders(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(borderSize, borderSize));
            h_CMY.Set_Borders(sf::Vector2f(borderSize, 0.0f), sf::Vector2f(2 * borderSize, borderSize));
            h_HSL.Set_Borders(sf::Vector2f(0.0f, borderSize), sf::Vector2f(borderSize, 2 * borderSize));
            h_HSB.Set_Borders(sf::Vector2f(borderSize, borderSize), sf::Vector2f(2 * borderSize, 2 * borderSize));
            shade_slider.Set_Borders({2 * borderSize + 20.0f, 20.0f}, {window_width - 20.0f, window_height - 12.0f});
            window.setView(sf::View(sf::FloatRect(0, 0, window_width, window_height)));
          }
          break;
        }

        case sf::Event::MouseButtonPressed:
          if (shade_slider.contains(mouse_pos)) {
            is_slider_dragged = true;
          }

          if (h_CMY.contains(mouse_pos)) {
            h_CMY.update_cmy_values(mouse_pos);
          }
          break;

        case sf::Event::MouseButtonReleased:
          is_slider_dragged = false;
          // h_RGB.Update_Texture();
          // h_CMY.Update_Texture();
          // h_HSL.Update_Texture();
          // h_HSB.Update_Texture();
          break;

        case sf::Event::MouseMoved:
          if (is_slider_dragged) {
            slider_val = shade_slider.update_value(mouse_pos);
            h_RGB.update_slider_value(slider_val);
            h_CMY.update_slider_value(slider_val);
            h_HSL.update_slider_value(slider_val);
            h_HSB.update_slider_value(slider_val);

            h_RGB.Update_Texture();
            h_CMY.Update_Texture();
            h_HSL.Update_Texture();
            h_HSB.Update_Texture();
          }

        default:
          break;
      }
    }

    window.draw(h_RGB);
    window.draw(h_CMY);
    window.draw(h_HSL);
    window.draw(h_HSB);
    window.draw(shade_slider);
    // Pewnie tu gdzieś wypadało by dorysować jeszcze suwaczek...
    text.setString(std::to_string(mean_frames_time) + "us");
    window.draw(text);

    // Pomiar czasu uśrednimy w okresie około 1/2 sekundy.
    frame_times.push_back(frame_clock.getElapsedTime().asMicroseconds());
    if (around_half_secound_clock.getElapsedTime().asSeconds() >= 0.5f && frame_times.size() >= 1) {
      mean_frames_time = (sf::Int64)((float)std::reduce(frame_times.begin(), frame_times.end()) / (float)frame_times.size());
      frame_times.clear();
      around_half_secound_clock.restart();
    }

    frame_clock.restart();  // Stop pomiaru czasu.
    window.display();
  }

  // Hmmm ... :-/
  font.reset();

  return 0;
}