/**
@file main.cpp
*/

/// Blok try {...} catch {..} służy do obsługi i przechwytywania wyjątków, 
/// co pozwala programowi kontynuować działanie pomimo wystąpienia błędów.
/// Jeżeli natrafi na błąd, będzie on wyświetlany.
#include "files/app.hpp"
int main() {
        
/// Chowanie CMD podczas uruchamiania aplikacji
#ifdef NDEBUG
    HWND hwnd = GetConsoleWindow();
    ShowWindow(hwnd, SW_HIDE);
#endif

    try {
        App app;
        app.run();
    }
    catch (std::exception &exception) {
        std::cerr << exception.what() << "\n";
    }
    return 0;
}
