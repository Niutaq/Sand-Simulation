#pragma once
#include "stuff.hpp"

/// Utworzenie klasy 'Menu' - jej parametry i metody
class Menu {
private:
  sf::RenderWindow* window;
  sf::RectangleShape* windowClose;
  sf::Texture* image;
  sf::Image* icon;
  sf::Sprite* background;

  sf::Vector2i posMouse;
  sf::Vector2f mouseCoord;

protected:
    void SetValues();
    void Events();
    void draw();

public:
    Menu();
    ~Menu();
    void run();
};