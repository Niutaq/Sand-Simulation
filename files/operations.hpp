#pragma once
#include "stuff.hpp"

class Op {
public:
	/// Konstruktor klasy 'Op'
	Op();
	/// Funkcja do wprowadzania danych do siatki (grid) i renderowania na oknie (window).
	void Input(std::vector<std::vector<int>>& grid, sf::RenderWindow& window);
	
	/// Funkcja do aktualizowania siatki (grid) w zależności od wysokości (worldheight) i szerokości (worldwidth) świata.
	void UpdateGrid(std::vector<std::vector<int>>& grid, int worldHeight, int worldWidth);
	
	/// Funkcja do rysowania siatki (grid) na oknie (window).
	void Draw(std::vector<std::vector<int>>& grid, sf::RenderWindow& window);
	
	// Funkcja tworząca proceduralny teren bazująca na "Perlin Noise"
	float GenTerrain(float x, float y);
	
	/// Funkcje do generowania losowych odcieni kolorów.
	sf::Color getRandomShadeAlpha(sf::Color baseColor);
	sf::Color getRandomShade(sf::Color baseColor);
	sf::Color ColorPallette();
};