/**
@file operations.cpp
*/

#include "stuff.hpp"

/// Użyte zmienne statyczne
extern Material Sand;
extern Material Stone;
extern Material Water;
extern Material Particle;
extern Material Acid;
extern Material Plant;
extern Material Erase;

Op::Op() {}

/// Funkcja do wprowadzania danych do siatki (grid) i renderowania na oknie (window).
void Op::Input(std::vector<std::vector<int>>& grid, sf::RenderWindow& window) {
	// ID określające dany materiał
	static int matId = 1;
    /// Zmienna do tworzenia okienek
	//Window win;
	//win.DrawWindow(grid, window);
	//win.HandleClickEvent(grid, window, matId);

	/// Piasek
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) {
		matId = 1;
	}		

	/// Kamień
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) {
		matId = 2;
	}

	/// Woda
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3)) {
		matId = 3;
	}

	/// Gaz
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4)) {
		matId = 4;
	}

	/// Kwas
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num5)) {
		matId = 5;
	}

	/// Roślina
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num6)) {
		matId = 6;
	}

	/// Usuwanie
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num7)) {
		matId = 7;
	}

	/// Deszcz
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {

		int xP = (rand() % World::worldWidth) % World::worldWidth;
		grid[0][xP] = 3;
	}		

	/// Czyszczenie ekranu
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) { 
		for (int i = World::worldHeight - 1; i >= 0; i--)
			for (int j = World::worldWidth - 1; j >= 0; j--)
				grid[i][j] = 0;
	}

	/// Zwiększenie kursora przy naciśnięciu "E"
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
		if (World::cursorSize < 4)
			World::cursorSize++;
	}

	/// Zmniejszenie kursora przy naciśnięciu "Q"
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
		if (World::cursorSize > 1)
			World::cursorSize--;
	}

	/// Oryginalne okno + zmienna do przeskalowania okna
	sf::Vector2i originalWindowSize(1000, 1000);
	sf::Vector2f scaleFactor(static_cast<float>(window.getSize().x) / originalWindowSize.x, static_cast<float>(window.getSize().y) / originalWindowSize.y);

	/// Wykrywanie pozycji myszy (jako pobranie jes pozycji przy użyciu funkcji .getPosition z biblioteki SFML)
	/// Lecz to nie koniec, poniewaz pozycja ta zeby zmieściła się w siatce N x M powinna być podzielona przez wielkość piksela
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
    	sf::Vector2i mousePos = sf::Mouse::getPosition(window) / World::pixelSize;

    	/// Skoro okno zostało przeskalowane, to i pozycja myszki musi zostać przeskalowana
    	mousePos.x /= scaleFactor.x;
		mousePos.y /= scaleFactor.y;

   		if (mousePos.y >= 0 && mousePos.y < World::worldHeight && mousePos.x >= 0 && mousePos.x < World::worldWidth) {
   			for (int i = 0; i < World::cursorSize && mousePos.y + i < World::worldHeight; i++) {
   		        for (int j = 0; j < World::cursorSize && mousePos.x + j < World::worldWidth; j++) {
   		            if (grid[mousePos.y + i][mousePos.x + j] == 0) {
   		                grid[mousePos.y + i][mousePos.x + j] = matId;
   		            }
   		    	}
   			}
   		}
	}
}

/// Nadanie poszczególnym ID danej funkcji wywołującej np. symulację przemieszczania się
void Op::UpdateGrid(std::vector<std::vector<int>>& grid, int worldHeight, 
				int worldWidth) {

	for (int i = World::worldHeight - 1; i >= 0; --i) {
		for (int j = World::worldWidth - 1; j >= 0; j--){
			switch (grid[i][j]) {
			case 0:
				break;
			case 1:
				Sand.UpdateMaterial(grid, i, j);
				Sand.Gravity(grid, i, j);
				break;
			case 2:
				break;
			case 3:
				Water.UpdateMaterialv2(grid, i, j);
				break;
			case 4:
				Particle.UpdateMaterialv3(grid, i, j);
				break;
			case 5:
				Acid.UpdateMaterialv4(grid, i, j);
				break;
			case 6:
				Plant.UpdateMaterialv5(grid, i, j);
				break;	
			case 7:
				Erase.Erase(grid, i, j);
				break;
			}
		}
	}
}

/// Losowanie odcieni danego koloru powołując się ponownie na losowanie liczb pseudolowosych (Mersenne Twister)
/// oraz przypisanie dla danego kanału r, g lub b (red, green, blue) obliczonego bazowego koloru (np. .r nowy =  r pierwotny(235) * 0.9)
sf::Color getRandomShadeAlpha(sf::Color baseColor) {
    std::random_device rd;
    
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> distribution(0.9, 1.0);
    sf::Color shadeColor;
    shadeColor.r = static_cast<sf::Uint8>(baseColor.r * distribution(gen));
    shadeColor.g = static_cast<sf::Uint8>(baseColor.g * distribution(gen));
    shadeColor.b = static_cast<sf::Uint8>(baseColor.b * distribution(gen));
    
    /// Kanał alpha : w tym przypadku jest to 76 z 255, czyli 30%
    shadeColor.a = 255;

    return shadeColor;
}

/// Podobnie jak wyżej, lecz bez przypisania kanału alpha, oznaczjącego poziom transparentności piksela
sf::Color Op::getRandomShade(sf::Color baseColor) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> distribution(0.9, 1.0);
    sf::Color shadeColor;
    shadeColor.r = static_cast<sf::Uint8>(baseColor.r * distribution(gen));
    shadeColor.g = static_cast<sf::Uint8>(baseColor.g * distribution(gen));
    shadeColor.b = static_cast<sf::Uint8>(baseColor.b * distribution(gen));
    
    return shadeColor;
}

/// Paleta kolorów
sf::Color Op::ColorPallette() {

	sf::Color c(235, 230, 100);
    return c;
}

/// Funkcja rysująca
void Op::Draw(std::vector<std::vector<int>>& grid, sf::RenderWindow& window) {

    for (int i = World::worldHeight - 1; i >= 0; i--) {
        for (int j = World::worldWidth - 1; j >= 0; j--) {
            switch (grid[i][j]) {
                case 1:
                	/// Nadanie pozycji pixelowi jako podstawowe 'x' i 'y' (czyli w tym przypadku 'i' i 'j') + dodanie rozmiarów temu pikselowi
                	/// jako pomnożenie jego pozycji przez zadany rozmiar piksela. Odpowiednie nadanie kololów pikselom. Rysowanie pikselów
                	/// Ten scenariusz występuje we wszystkich przypadkach
                	if (i % 2 == 0 && j % 3 == 0) {
                    	Env::sand.setPosition(static_cast<float>(j * World::pixelSize), static_cast<float>(i * World::pixelSize));
                    	Env::sand.setFillColor(sf::Color(195, 152, 3));
                    	window.draw(Env::sand);
                	} else if (i % 3 == 0 && j % 4 == 0) {
                		Env::sand.setPosition(static_cast<float>(j * World::pixelSize), static_cast<float>(i * World::pixelSize));
                    	Env::sand.setFillColor(sf::Color(195, 153, 4));
                    	window.draw(Env::sand);
                    } else if (i % 4 == 0 && j % 5 == 0) {
                		Env::sand.setPosition(static_cast<float>(j * World::pixelSize), static_cast<float>(i * World::pixelSize));
                    	Env::sand.setFillColor(sf::Color(195, 155, 5));
                    	window.draw(Env::sand);
                    } else if (i % 5 == 0 && j % 7 == 0) {
                		Env::sand.setPosition(static_cast<float>(j * World::pixelSize), static_cast<float>(i * World::pixelSize));
                    	Env::sand.setFillColor(sf::Color(195, 157, 6));
                    	window.draw(Env::sand);
                    } else {
                    	Env::sand.setPosition(static_cast<float>(j * World::pixelSize), static_cast<float>(i * World::pixelSize));
                    	Env::sand.setFillColor(sf::Color(195, 148, 7));
                    	window.draw(Env::sand);
                    }
                    break;

                case 2:
                    Env::stone.setPosition(static_cast<float>(j * World::pixelSize), static_cast<float>(i * World::pixelSize));
                    Env::stone.setFillColor(sf::Color(128, 128, 128));
                    window.draw(Env::stone);
                    break;
                case 3:
                    Env::water.setPosition(static_cast<float>(j * World::pixelSize), static_cast<float>(i * World::pixelSize));
                    Env::water.setFillColor(getRandomShade(sf::Color::Blue));
                    window.draw(Env::water);
                    break;

                case 4:
                    Env::particle.setPosition(static_cast<float>(j * World::pixelSize), static_cast<float>(i * World::pixelSize));
                    Env::particle.setFillColor(getRandomShade(sf::Color::White));
                    window.draw(Env::particle);
                    break;

                case 5:
                    Env::acid.setPosition(static_cast<float>(j * World::pixelSize), static_cast<float>(i * World::pixelSize));
                    Env::acid.setFillColor(getRandomShade(sf::Color(134, 22, 168)));
                    window.draw(Env::acid);
                    break;

                case 6:
                   if (i % 2 == 0 && j % 3 == 0) {
                    	Env::plant.setPosition(static_cast<float>(j * World::pixelSize), static_cast<float>(i * World::pixelSize));
                    	Env::plant.setFillColor(sf::Color(32, 138, 21));
                    	window.draw(Env::plant);
                	} else if (i % 3 == 0 && j % 4 == 0) {
                		Env::plant.setPosition(static_cast<float>(j * World::pixelSize), static_cast<float>(i * World::pixelSize));
                    	Env::plant.setFillColor(sf::Color(35, 142, 21));
                    	window.draw(Env::plant);
                    } else if (i % 4 == 0 && j % 5 == 0) {
                		Env::plant.setPosition(static_cast<float>(j * World::pixelSize), static_cast<float>(i * World::pixelSize));
                    	Env::plant.setFillColor(sf::Color(40, 144, 26));
                    	window.draw(Env::plant);
                    } else if (i % 5 == 0 && j % 7 == 0) {
                		Env::plant.setPosition(static_cast<float>(j * World::pixelSize), static_cast<float>(i * World::pixelSize));
                    	Env::plant.setFillColor(sf::Color(45, 146, 32));
                    	window.draw(Env::plant);
                    } else {
                    	Env::plant.setPosition(static_cast<float>(j * World::pixelSize), static_cast<float>(i * World::pixelSize));
                    	Env::plant.setFillColor(sf::Color(50, 138, 30));
                    	window.draw(Env::plant);
                    }
                    break;

                case 7:
                    Env::erase.setPosition(static_cast<float>(j * World::pixelSize), static_cast<float>(i * World::pixelSize));
                    Env::erase.setFillColor(sf::Color(0, 0, 25));
                    window.draw(Env::erase);
                    break;
            }

        }
    }

    window.display();
}

/// Funkcja tworząca proceduralny teren (losowo)
float Op::GenTerrain(float x, float y) {
	FastNoiseLite noise;
	noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2S);

	float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	noise.SetFrequency(5 + r  * 50);

	return noise.GetNoise(x, y);
}