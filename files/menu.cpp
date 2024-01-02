/**
@file menu.cpp
*/

#include "menu.hpp"
#include "menu_data.hpp"
#include "icon_data_2.hpp"

/// Konstruktor 'Menu'
Menu::Menu(){
  window = new sf::RenderWindow();
  windowClose = new sf::RectangleShape();
  image = new sf::Texture();
  icon = new sf::Image();
  background = new sf::Sprite();

  SetValues();
}

/// Destruktor 'Menu'
Menu::~Menu(){
  delete window;
  delete windowClose;
  delete image;
  delete icon;
  delete background;
}

void Menu::SetValues() {
  window->create(sf::VideoMode(1000, 1000), "Symulowane Piksele", sf::Style::Titlebar | sf::Style::Close);

  /// Tlo okna menu - bezpośrednie ładowanie z bajtów danych
  if (image->loadFromMemory(&menu_png, menu_png_len)) {
      background->setTexture(*image);
  } else {
      throw std::runtime_error("Tlo okna nie zostalo znalezione!");
  }

  /// Ładowanie ikony bezpośrednio z bajtów danych - bezpośrednie ładowanie z bajtów danych
  if (icon->loadFromMemory(&icon_bmp_2, icon_bmp_2_len)) {
      window->setIcon(icon->getSize().x, icon->getSize().y, icon->getPixelsPtr());
  }
  else {
      throw std::runtime_error("Ikona okna nie zostala znaleziona!");
  }

  /// Koordynaty myszki
  posMouse = {0,0};
  mouseCoord = {0,0};

  /// 'X' - wylaczenie menu
  windowClose->setSize(sf::Vector2f(50, 50));
  windowClose->setPosition(900, 50);
}


/// Uwzglednianie eventow
void Menu::Events() {
  sf::Event event;
  while (window->pollEvent(event)){
    if (event.type == sf::Event::Closed){
      window->close();
    }

    /// Pobranie pozycji myszki oraz zmapowanie tej pozycji na koordynaty
    posMouse = sf::Mouse::getPosition(*window);
    mouseCoord = window->mapPixelToCoords(posMouse);

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
      if (windowClose->getGlobalBounds().contains(mouseCoord)) {
         window->close();
      }
    } else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R) {
        window->close();
    }


  }
}

/// Rysowanie tla oraz wyswietlanie
void Menu::draw() {
  window->clear();
  window->draw(*background);
  window->display();
}

/// Uruchamianie wszystkiego
void Menu::run(){
  while (window->isOpen()) {
    Events();
    draw();
  }
}