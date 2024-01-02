/**
@file environment.cpp
*/

#include "stuff.hpp"

/// Klasa 'Env', która przetrzymuje statyczne zmienne określające typ obiektu piksela
sf::RectangleShape Env::sand = sf::RectangleShape(sf::Vector2f(static_cast<float>(World::pixelSize), static_cast<float>(World::pixelSize)));
sf::RectangleShape Env::stone = sf::RectangleShape(sf::Vector2f(static_cast<float>(World::pixelSize), static_cast<float>(World::pixelSize)));
sf::RectangleShape Env::water = sf::RectangleShape(sf::Vector2f(static_cast<float>(World::pixelSize), static_cast<float>(World::pixelSize)));
sf::RectangleShape Env::particle = sf::RectangleShape(sf::Vector2f(static_cast<float>(World::pixelSize), static_cast<float>(World::pixelSize * 2)));
sf::RectangleShape Env::acid = sf::RectangleShape(sf::Vector2f(static_cast<float>(World::pixelSize), static_cast<float>(World::pixelSize)));
sf::RectangleShape Env::plant = sf::RectangleShape(sf::Vector2f(static_cast<float>(World::pixelSize), static_cast<float>(World::pixelSize * 2)));
sf::RectangleShape Env::erase = sf::RectangleShape(sf::Vector2f(static_cast<float>(World::pixelSize), static_cast<float>(World::pixelSize)));