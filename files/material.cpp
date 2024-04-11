/**
@file material.cpp
*/

#include "stuff.hpp"

/// Konstruktor klasy 'Material'
Material::Material(std::string name, int id) : m_name(name), m_id(id) {}

/// Funkcja, dzięki której piasek z wodą odpowiednio współgra - istnieje pewna grawitacja
void Material::Gravity(std::vector<std::vector<int>> &grid, int i, int j) {
    /// Zwolnienie opadania piasku na dno, losowe rozprzestrzenianie się

    /// Kod używa generatora liczb losowych Mersenne Twister (std::mt19937), który jest zainicjowany 
    /// losową wartością z urządzenia generującego losowe liczby (std::random_device). 
    /// Następnie wykorzystuje rozkład równomierny, aby losowo wybrać liczbę 0 lub 1 i przypisuje ją do zmiennej direction.
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribution(0, 1); /// 0 dla lewej, 1 dla prawej strony
    int r = distribution(gen);
    /// Jeżeli pozycja znajduje się wewnątrz okna programu i na dole jest woda - to przesuń wodę do góry a piasek na dno
    if (r == 0 && i + 1 < World::worldHeight && j + 1 < World::worldWidth && grid[i + 1][j] == 3) {
        grid[i][j] = 3;
        grid[i + 1][j] = 1;
    } 
}

/// Symulacja materiału piasku
void Material::UpdateMaterial(std::vector<std::vector<int>> &grid, int i, int j) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribution(0, 1); /// 0 dla lewej, 1 dla prawej strony
    int direction = distribution(gen);

    /// Nie przekracza wymiarów (tj. spadek w dół, więc mie może przekroczyć wysokości siatki)
    /// i na dole jest pusto 
    if (i + 1 < World::worldHeight && grid[i + 1][j] == 0)
    {
        grid[i][j] = 0;
        grid[i + 1][j] = 1;
    }

    /// Przesuń w dół w prawo
    /// Dla zmiennej 'direction' = 0, Nie przekracza wymiarów (wysokość i szerokość), prawo-dół i prawo jest puste
    else if (direction == 0 && i + 1 < World::worldHeight && j + 1 < World::worldWidth && grid[i + 1][j + 1] == 0 && grid[i][j + 1] == 0) {
        grid[i][j] = 0;
        grid[i + 1][j + 1] = 1;
    }
    
    /// Przesuń w dół w lewo
    /// Dla zmiennej 'direction' = 1, Nie przekracza wymiarów (wysokość i szerokość), lewo-dół i lewo jest puste
    else if (direction == 1 && i + 1 < World::worldHeight && j > 0 && grid[i + 1][j - 1] == 0 && grid[i][j - 1] == 0) {
        grid[i][j] = 0;
        grid[i + 1][j - 1] = 1;
    }
}

/// Symulacja materiału wody
void Material::UpdateMaterialv2(std::vector<std::vector<int>> &grid, int i, int j) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribution(-1, 1); /// -1 dla lewej, 0 dla braku ruchu, 1 dla prawej strony
    int direction = distribution(gen);
    /// Nie przekracza wymiarów (tj. spadek w dół, więc mie może przekroczyć wysokości siatki)
    if (i + 1 < World::worldHeight) {
        /// Przesuń w dół, jeśli jest puste miejsce
        if (grid[i + 1][j] == 0) {
            grid[i][j] = 0;
            grid[i + 1][j] = 3; /// Woda spływa na dół
        } else {     
            /// Dla zmiennej 'direction' = -1, wolna jest lewa strona (nie przekracza lewej granicy), lewo jest puste
            if (direction == -1 && j > 0 && grid[i][j - 1] == 0) {
                grid[i][j] = 0;
                grid[i][j - 1] = 3; /// Ruch w lewo
            }
            /// Dla zmiennej 'direction' = 1, wolna jest prawa strona (nie przekracza prawej granicy), prawo jest puste
            else if (direction == 1 && j + 1 < World::worldWidth && grid[i][j + 1] == 0) {
                grid[i][j] = 0;
                grid[i][j + 1] = 3; /// Ruch w prawo
            }
            else if (direction == 0 && j + 1 < World::worldWidth && (grid[i + 1][j] == 5 || grid[i + 1][j - 1] == 5 || grid[i + 1][j + 1] == 5)) {
                grid[i + 1][j] = 3;
                //grid[i + 1][j + 1] = 3; 
                //grid[i + 1][j - 1] = 3;
            }
        }
    }
}

/// Symulacja cząstek
void Material::UpdateMaterialv3(std::vector<std::vector<int>>& grid, int i, int j) {
    if (i >= 0 && i < World::worldHeight && j >= 0 && j < World::worldWidth) {
        int liveNeighbors = 0; /// Liczba żywych 'sąsiadów'

        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                if (dx == 0 && dy == 0) continue; /// Pominięcie komórki w centrum
                int ni = (i + dx + World::worldHeight) % World::worldHeight; /// Nowa pozycja 'i' komórki
                int nj = (j + dy + World::worldWidth) % World::worldWidth;   /// Nowa pozycja 'j' komórki
                if (grid[ni][nj] == 4 || grid[ni][nj] == 0) { /// Zastosowanie '|| grid[ni][nj] == 0' - cząstki zjadają inne piksele i się rozmnażają
                    liveNeighbors++; /// Wzrasta liczba 'sąsiadów'
                } else if (grid[ni][nj] != 6) {
                    liveNeighbors += 2; /// Znacznie wzrasta liczba 'sąsiadów', gdy cząstki napotkają roślinę. UWAGA ! wygląda to nielogicznie, ale działa :)
                }
            }
        }

        int direction = std::rand() % 4; /// Losowy kierunek

        int newI = i;
        int newJ = j;

        if (direction == 0) {
            newI = (i - 1 + World::worldWidth) % World::worldWidth + 1/2;   /// Ruch do góry
        } else if (direction == 1) {
            newI = (i + 1 + World::worldHeight) % World::worldWidth + 1/2;  /// Ruch w dół
        } else if (direction == 2) {
            newJ = (j - 1 + World::worldHeight) % World::worldHeight + 1/2; /// Ruch w lewo
        } else {
            newJ = (j + 1 + World::worldHeight) % World::worldHeight + 1/2; /// Ruch w prawo
        }

        /// Warunek, aby cząstki nie przekraczały : górnej i dolnej 'granicy' mapy
        if (newI <= 0) {
            i = 0;  
            newI = 0;
        } else if (newI >= World::worldHeight - 1) {
            i = World::worldHeight - 1;
            newI = World::worldHeight - 1;
        }

        /// Jeżeli na pozycji piksela pojawi się kwas to to cząstką ginie
        if (grid[i][j] == 5 || grid[newI][newJ] == 5) {
            grid[i][j] = 0;
            grid[newI-1][newJ-1] = 0;

        } else if (grid[newI][newJ] == 2) {  // Kolizja z kamieniem

        /// Losowo wybierz kierunek: 0 to pionowo, 1 to poziomo
        int newDir = rand() % 4;
    
        if (newDir == 0) {
            /// Próba przesunięcia się pionowo
            int upDown = rand() % 2; /// 0 dla góry, 1 dla dołu
    
            /// Czy nowa pozycja mieści się w zakresie
            if (upDown == 0 && i > 0) {
                grid[i][j] = 0; /// Wyczyść bieżącą komórkę
                grid[i - 1][j] = 4; /// Przesuń cząstkę w górę
            } else if (upDown == 1 && i < World::worldHeight - 1) {
                grid[i][j] = 0; /// Wyczyść bieżącą komórkę
                grid[i + 1][j] = 4; /// Przesuń cząstkę w dół
            }
    
        } else {
            /// Spróbuj przesunąć się poziomo
            int leftRight = rand() % 2; /// 0 dla lewo, 1 dla prawo
    
            /// Sprawdź, czy nowa pozycja mieści się w zakresie
            if (leftRight == 0 && j > 0) {
                grid[i][j] = 0; /// Wyczyść bieżącą komórkę
                grid[i][j - 1] = 4; /// Przesuń cząstkę w lewo
            } else if (leftRight == 1 && j < World::worldWidth - 1) {
                grid[i][j] = 0; /// Wyczyść bieżącą komórkę
                grid[i][j + 1] = 4; /// Przesuń cząstkę w prawo
            }
        }

        } else {
            grid[newI][newJ] = 4; /// Ustaw nowy stan komórki
        }
    
        /// Zastosowanie zasad 'Gry w życie' Conway'a
        if (grid[i][j] == 4) {
            if (liveNeighbors < 2 || liveNeighbors > 3) {
                /// Komórka umiera z powodu zbyt małej lub zbyt dużej liczby sąsiadów
                grid[i][j] = 0;
            }
        } else if (grid[i][j] == 0) {
            if (liveNeighbors == 3) {
                /// Martwa komórka ożywa
                grid[i][j] = 4;
            }
        }


    }
}

/// Symulacja kwasu
void Material::UpdateMaterialv4(std::vector<std::vector<int>> &grid, int i, int j) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribution(-1, 1); /// -1 dla lewej, 0 dla braku ruchu, 1 dla prawej strony
    int direction = distribution(gen);
    /// Nie przekracza wymiarów (tj. spadek w dół, więc mie może przekroczyć wysokości siatki)
    if (i + 1 < World::worldHeight) {
        /// Przesuń w dół, jeśli jest puste miejsce
        if (grid[i + 1][j] == 0) {
            grid[i][j] = 0;
            grid[i + 1][j] = 5; /// Kwas spływa na dół
        } else {     
            /// Dla zmiennej 'direction' = -1, wolna jest lewa strona (nie przekracza lewej granicy), lewo jest puste
            if (direction == -1 && j > 0 && grid[i][j - 1] == 0) {
                grid[i][j] = 0;
                grid[i][j - 2] = 5; /// Ruch w lewo
            }
            /// Dla zmiennej 'direction' = 1, wolna jest prawa strona (nie przekracza prawej granicy), prawo jest puste
            else if (direction == 1 && j + 1 < World::worldWidth && grid[i][j + 1] == 0) {
                grid[i][j] = 0;
                grid[i][j + 2] = 5; /// Ruch w prawo

            } else if (direction == 0 && j + 1 < World::worldWidth && (grid[i - 1][j + 1] == 0 || grid[i - 1][j - 1] || grid[i][j])) {
                /// Zwolnienie opadania kwasu na dno, losowe rozprzestrzenianie się
                int r = rand() % 3;
                 /// Jeżeli pozycja znajduje się wewnątrz okna programu i na dole jest jaki kolwiek materiał - to kwas wchłania ten materiał
                if (r == 0 && i + 1 < World::worldHeight && j + 1 < World::worldWidth && grid[i + 1][j]) {
                    grid[i + 1][j] = 5;
                } 
            }
        }
    }
}

/// Symulacja roślin
void Material::UpdateMaterialv5(std::vector<std::vector<int>>& grid, int i, int j) {
    /// Sprawdzenie, czy komórka nad aktualną zawiera wodę i czy aktualna komórka ma ID rośliny.
    /// Jeśli tak, to roślina rośnie do góry.
    int r = rand() % 4;
    if (r == 0 && i > 0 && grid[i - 1][j] == 3 && grid[i][j] == 6) {
        /// Wzrost rośliny do góry (na przykład inkrementacja ID rośliny lub zmiana koloru piksela).
        grid[i - 1][j] = 6; /// Aktualizacja komórki na roślinę.
    }

    /// Sprawdzenie, czy komórka po lewej stronie aktualnej zawiera wodę i czy aktualna komórka ma ID rośliny.
    /// Jeśli tak, to roślina rośnie na boki.
    if (r == 1 && j > 0 && grid[i][j - 1] == 3 && grid[i][j] == 6) {
        /// Wzrost rośliny na boki (analogicznie jak w przypadku wzrostu do góry).
        grid[i][j - 1] = 6; /// Aktualizacja komórki na roślinę.
    }

    /// Sprawdzenie, czy komórka poniżej aktualnej jest wodą - jeśli to możliwe. Jeśli tak, to roślina rośnie w dół.
    if (r == 2 && i < World::worldHeight - 1 && grid[i + 1][j] == 3 && grid[i][j] == 6) {
        /// Wzrost rośliny w dół.
        grid[i + 1][j] = 6; /// Aktualizacja komórki na roślinę.
    }

    /// Sprawdzenie, czy komórka po prawej stronie aktualnej jest wodą - jeśli to możliwe. Jeśli tak, to roślina rośnie w prawo.
    if (r == 3 && j < World::worldWidth - 1 && grid[i][j + 1] == 3 && grid[i][j] == 6) {
        /// Wzrost rośliny w prawo.
        grid[i][j + 1] = 6; /// Aktualizacja komórki na roślinę.
    } 
}

/// Usuwanie pikeseli jako rysowanie 'pustych pikseli'
void Material::Erase(std::vector<std::vector<int>> &grid, int i, int j) {
    if (i >= 0 && i < World::worldHeight && j >= 0 && j < World::worldWidth) {
        /// Potrzeba wyzerować obecny pixel, ponieważ inaczej usuwanie będzie działać, lecz obecny piksel będzie 'niewidzialny', którego nie da rady będzie usunąć
        grid[i][j] = 0;

        /// Usuwanie polegan na sprawdzeniu danego materiału piksela, czy kursor nie przekroczyl ponownie siatki oraz usuwanie ma kształy na wygląd znaku '+'
        if (i + 1 < World::worldHeight && (grid[i + 1][j] == 1 || grid[i - 1][j] == 1)) { /// Piasek
            grid[i + 1][j] = 0;
            grid[i - 1][j] = 0;
            grid[i][j + 1] = 0;
            grid[i][j - 1] = 0;
        }

        if (i + 1 < World::worldHeight && (grid[i + 1][j] == 2 || grid[i - 1][j] == 2)){ /// Kamień
            grid[i + 1][j] = 0;
            grid[i - 1][j] = 0;
            grid[i][j + 1] = 0;
            grid[i][j - 1] = 0;
        }

        if (i + 1 < World::worldHeight && (grid[i + 1][j] == 3 || grid[i - 1][j] == 3)) { /// Woda
            grid[i + 1][j] = 0;
            grid[i - 1][j] = 0;
            grid[i][j + 1] = 0;
            grid[i][j - 1] = 0;
        }

        if (i + 1 < World::worldHeight && (grid[i + 1][j] == 4 || grid[i - 1][j] == 4)) { /// Cząstka
            grid[i + 1][j] = 0;
            grid[i - 1][j] = 0;
            grid[i][j + 1] = 0;
            grid[i][j - 1] = 0;
        }

        if (i + 1 < World::worldHeight && (grid[i + 1][j] == 5 || grid[i - 1][j] == 5)) { /// Kwas
            grid[i + 1][j] = 0;
            grid[i - 1][j] = 0;
            grid[i][j + 1] = 0;
            grid[i][j - 1] = 0;
        }

        if (i + 1 < World::worldHeight && (grid[i + 1][j] == 6 || grid[i - 1][j] == 6)) { /// Roślina
            grid[i + 1][j] = 0;
            grid[i - 1][j] = 0;
            grid[i][j + 1] = 0;
            grid[i][j - 1] = 0;
        }
    }
}