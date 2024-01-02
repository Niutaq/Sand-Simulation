#pragma once 
#include "stuff.hpp"

/// Klasa 'Env', która przetrzymuje statyczne zmienne określające typ obiektu piksela
class Env {

public:
	static sf::RectangleShape sand;
	static sf::RectangleShape stone;
	static sf::RectangleShape water;
	static sf::RectangleShape particle;
	static sf::RectangleShape acid;
	static sf::RectangleShape plant;
	static sf::RectangleShape erase;

};