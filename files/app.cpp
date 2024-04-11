/**
@file app.cpp
*/

#include "stuff.hpp"

#include "font_data.hpp"
#include "icon_data.hpp"
#include "mountains_data.hpp"
#include "pyramids_data.hpp"
#include "menu_data_2.hpp"

/// Materiały
Material Sand("Sand", 1);
Material Stone("Stone", 2);
Material Water("Water", 3);
Material Particle("Particle", 4);
Material Acid("Acid", 5);
Material Plant("Plant", 6);
Material Erase("Erase", 7);

/// Konstruktor
App::App() {}

void App::run() {
    /// Zwalnianie zasobów związanych z konsolą systemową
    FreeConsole();

    /// Obiekt klasy 'Menu' tworzący menu aplikacji
    Menu* menu = new Menu();
    menu->run();
    delete menu;
    menu = nullptr;

    /// Obiekt klasy 'Op'
    Op op;
	/// Okno programu
	sf::RenderWindow window(sf::VideoMode(World::width, World::height), "Piksele Symulowane");
	/// 'Tablica tablic', czyli macierz o określonej wielkości / siatka N x M pikseli | '0' oznacza pustą macierz - piksele o ID=0
	std::vector<std::vector<int>> grid(World::worldHeight, std::vector<int>(World::worldWidth, 0));
	
	/// Zmienna od liczenia czasu, ustawienie liczby FPS oraz VSync
	sf::Clock clock;
	window.setFramerateLimit(60);
	window.setVerticalSyncEnabled(true);

    /// Ładowanie czczonki bezpośrednio z bajtów danych
    sf::Font fromMemoryFont;
    fromMemoryFont.loadFromMemory(&PixelifySans_Regular_ttf, PixelifySans_Regular_ttf_len);
    sf::Text fpsText("", fromMemoryFont);
    /// Informacja o wciśnieciu przycisku 'H'
    sf::Text helperInfo;

    if (!fromMemoryFont.loadFromMemory(&PixelifySans_Regular_ttf, PixelifySans_Regular_ttf_len)) {
        throw std::runtime_error("Czcionka nie zostala znaleziona!");
    }

    /// Ładowanie ikony bezpośrednio z bajtów danych
    sf::Image fromMemoryIcon;
    if (fromMemoryIcon.loadFromMemory(&icon_bmp, icon_bmp_len)) {
        window.setIcon(fromMemoryIcon.getSize().x, fromMemoryIcon.getSize().y, fromMemoryIcon.getPixelsPtr());
    }
    else {
        throw std::runtime_error("Ikona okna nie zostala znaleziona!");
    }

 	/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    /// Ładowanie tła ekranu
    /// Obsługa zdarzeń klawiatury
    sf::Image backgroundImageFromMemory;
    sf::Image helperBackground;
    /// Ustawienie zdjęcia jako tekstury
    sf::Texture bgTexture;
    sf::Texture bgTextureHelper;
    /// Utworzenie 'obiektu' jako możwliwość wyświetlenia obrazu
    sf::Sprite bgSprite;
    sf::Sprite bgSpriteHelper;
    /// Zmienna sprawdzajaca zamiane tla okna 
    bool ifChange = true;
    /// Zmienna sprawdzajaca zamiane tla okna pomocniczego 
    bool ifChange2 = false;

    /// Ustawienie czcionki dla tekstu wyświetlania FPS, pozycja i rozmiar
    /// Ustawienie czcionki dla tekstu wyświetlania pomocy, pozycja, rozmiar oraz kolor
    fpsText.setFont(fromMemoryFont);
    fpsText.setPosition((World::width - (World::width / 6)), 7.5);
    fpsText.setCharacterSize(45);
    helperInfo.setFont(fromMemoryFont);
    helperInfo.setPosition((World::width - (World::width / 4.5)), 48);
    helperInfo.setCharacterSize(45);
    helperInfo.setFillColor(sf::Color(245, 40, 40));

    /// Pierwiotne tlo
    if (!backgroundImageFromMemory.loadFromMemory(&mountains_png, mountains_png_len)) {
        throw std::runtime_error("Inne tło nie zostało znalezione!");
    } else {
        backgroundImageFromMemory.loadFromMemory(&mountains_png, mountains_png_len);
        /// Ustawienie poziomu przeźroczystości dla zdjęcia
        for (int x = 0; x < World::width; x++) {
            for (int y = 0; y < World::height; y++) {
                sf::Color pixel = backgroundImageFromMemory.getPixel(x, y);
                pixel.a = 122.5 / 2; /// 25% alpha (0-255)
                backgroundImageFromMemory.setPixel(x, y, pixel);
            }
        }
        bgTexture.loadFromImage(backgroundImageFromMemory);
    }

	/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    /// Oryginalne okno + zmienna do przeskalowania okna
    sf::Vector2i originalWindowSize(1000, 1000);
    sf::Vector2f scaleFactor(static_cast<float>(window.getSize().x) / originalWindowSize.x, static_cast<float>(window.getSize().y) / originalWindowSize.y);
    /// Nieskończona pętla dopóki okno programu nie zostanie wyłączone lub wciśnięty przycisk ESC
    while (window.isOpen() && !sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
        /// 'Event' - zmienna do odczytywania 'wydarzeń'/ zbieranie informacji np. z klawiatury
        sf::Event event;
        while (window.pollEvent(event))
        {  /// Jeżeli nastąpi event - 'zamknięte', to okno się zamknie
            if (event.type == sf::Event::Closed)
                window.close();
            /// Jeżeli nastąpi event - 'zmiana rozmiaru okna', to nastąpi przeskalowanie okna
            if (event.type == sf::Event::Resized) {
                sf::Vector2u newWindowSize = window.getSize();
                scaleFactor.x = static_cast<float>(newWindowSize.x) / originalWindowSize.x;
                scaleFactor.y = static_cast<float>(newWindowSize.y) / originalWindowSize.y;
            }
            /// Jeżeli nastąpi event - 'naciśnięcie klawisza', to zmień tło na inne zdjęcie
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::I) {
                if (ifChange) {
                    if (!backgroundImageFromMemory.loadFromMemory(&pyramids_png, pyramids_png_len)) {
                        throw std::runtime_error("Inne tlo nie zostalo znalezione!");
                    }
                }
                else {
                    if (!backgroundImageFromMemory.loadFromMemory(&mountains_png, mountains_png_len)) {
                        throw std::runtime_error("Tekstura tla ekranu nie zostala znaleziona!");
                    }
                }

                /// Ustawienie poziomu przeźroczystości dla zdjęcia
                for (int x = 0; x < World::width; x++) {
                    for (int y = 0; y < World::height; y++) {
                        sf::Color pixel = backgroundImageFromMemory.getPixel(x, y);
                        pixel.a = 122.5 / 2; /// 25% alpha (0-255)
                        backgroundImageFromMemory.setPixel(x, y, pixel);
                    }
                }

                /// Nadanie zdjęcia w tle do tekstury a później do obiektu
                bgTexture.loadFromImage(backgroundImageFromMemory);
                bgSprite.setTexture(bgTexture);
                ifChange = !ifChange;
            }

            /// Okno z informacjami o grze
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::H) {
                if (ifChange2) {
                    if (!helperBackground.loadFromMemory(&menu_pomoc_png, menu_pomoc_png_len)) {
                        throw std::runtime_error("Tło pomocnicze nie zostało znalezione!");
                    }
                }

                /// Nadanie zdjęcia w tle do tekstury a później do obiektu
                ifChange2 = !ifChange2;

                helperBackground.loadFromMemory(&menu_pomoc_png, menu_pomoc_png_len);
                bgTextureHelper.loadFromImage(helperBackground);

                if (ifChange2) { bgSpriteHelper.setTexture(bgTextureHelper); }
                else bgSpriteHelper.setTexture(bgTexture);
            }

        }

        /// Jeżeli naciśnie się klawisz 'P', to można 'wygenerować' testową planszę
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::P)) {
            for (int x = World::width / 10; x < World::worldHeight - World::height / 100; x++) {
                if (x < 0 || x >= World::worldHeight) continue;
                for (int y = World::width / 100; y < World::worldWidth - World::width / 100; y++) {
                    if (y < 0 || y >= World::worldWidth) continue;

                    float noiseValue = op.GenTerrain(static_cast<float>(x), static_cast<float>(y));

                    if (noiseValue > 0.4 && noiseValue < 0.45) {
                        grid[x][y] = 2;
                    }
                    else if (noiseValue > 0.45 && noiseValue < 0.455) {
                        grid[x][y] = 6;
                    }
                    else if (noiseValue > 0.7 && noiseValue < 0.8) {
                        grid[x][y] = 1;
                    }
                    else if (noiseValue > 0.8) {
                        grid[x][y] = 3;
                    }
                }
            }
        }

            /// Pobieranie uchwytu
            op.Input(grid, window);

            /// Aktualizacja pikseli pomiędzy różnymi materiałami
            op.UpdateGrid(grid, World::worldHeight, World::worldWidth);

            /// Rysowanie
            op.Draw(grid, window);

            /// Wywołanie zwraca czas od ostatniego pomiaru, a następnie resetuje zegar, aby rozpocząć pomiar od nowa. 
            sf::Time time = clock.restart();
            /// Dzieląc 1 przez czas w sekundach (uzyskany za pomocą asSeconds()), otrzymuje się ilość klatek na sekundę. Wynik jest przechowywany w zmiennej fps jako liczba zmiennoprzecinkowa.
            float fps = 1.0f / time.asSeconds();
            /// Wypisanie FPS jako string, 'std::flush' służy do 'opróżniania' sekwencji wypisywanej w konsoli
            std::cout << "FPS : " << std::to_string(fps) << "\r" << std::flush; 
            /// Odświeżanie okna programu oraz rysowanie określonych elementów
            window.clear();
            window.draw(bgSprite);
            window.draw(bgSpriteHelper);

            /// Nadanie liczby FPS do tekstu wyświetlanego w oknie
            if (ifChange2) { fpsText.setString(""); }
            else { fpsText.setString("FPS: " + std::to_string(static_cast<int>(fps))); }

            window.draw(fpsText);

            /// Nadanie liczby FPS do tekstu wyświetlanego w oknie
            if (ifChange2) { helperInfo.setString(""); }
            else { helperInfo.setString("Pomoc - h"); }
            window.draw(helperInfo);
   }    	
}