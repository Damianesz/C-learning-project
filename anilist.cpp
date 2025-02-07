#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <windows.h>
#include <commdlg.h>

class Anime {
public:
    std::string Nazwa;
    int Rok_produkcji;
    int Liczba_odcinkow;
    bool Rewatch;
    std::string SciezkaObrazu;

    void Wprowadzenie() const {
        std::cout << "Nazwa: " << Nazwa << std::endl;
        std::cout << "Rewatch: " << (Rewatch ? "Tak" : "Nie") << std::endl;
        std::cout << "Liczba odcinków: " << Liczba_odcinkow << std::endl;
        std::cout << "Rok produkcji: " << Rok_produkcji << std::endl;
        std::cout << "Ścieżka obrazu: " << SciezkaObrazu << std::endl;
    }

    // Przeciążenie operatora << do zapisu obiektu Anime
    friend std::ostream& operator<<(std::ostream& os, const Anime& anime) {
        os << anime.Nazwa << '\n'
           << anime.Rok_produkcji << '\n'
           << anime.Liczba_odcinkow << '\n'
           << anime.Rewatch << '\n'
           << anime.SciezkaObrazu;
        return os;
    }

    // Przeciążenie operatora >> do odczytu obiektu Anime
    friend std::istream& operator>>(std::istream& is, Anime& anime) {
        std::getline(is, anime.Nazwa);
        is >> anime.Rok_produkcji;
        is >> anime.Liczba_odcinkow;
        is >> anime.Rewatch;
        is.ignore(); // Czyszczenie bufora przed kolejną linią
        std::getline(is, anime.SciezkaObrazu);
        return is;
    }
};

std::string OtworzEksploratorPlikow() {
    OPENFILENAMEA ofn;
    CHAR szPlik[260] = { 0 };

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.lpstrFile = szPlik;
    ofn.nMaxFile = sizeof(szPlik);
    ofn.lpstrFilter = "Obrazy\0*.jpg;*.jpeg;*.png;*.bmp\0Wszystkie pliki\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

    if (GetOpenFileNameA(&ofn)) {
        return ofn.lpstrFile;
    }
    return "";
}

void WczytajZPliku(const std::string& nazwaPliku, std::vector<Anime>& animeList) {
    std::ifstream Odczyt(nazwaPliku);
    if (Odczyt.is_open()) {
        Anime anime;
        while (Odczyt >> anime) {
            animeList.push_back(anime);
            Odczyt.ignore(); // Ignorowanie pustej linii po każdym rekordzie
        }
        Odczyt.close();
    }
}

void ZapiszDoPliku(const std::string& nazwaPliku, const std::vector<Anime>& animeList) {
    std::ofstream Zapis(nazwaPliku, std::ios::out);
    if (Zapis.is_open()) {
        for (const auto& anime : animeList) {
            Zapis << anime << "\n\n"; // Dodanie pustej linii między rekordami
        }
        Zapis.close();
    } else {
        std::cout << "Nie udało się otworzyć pliku do zapisu.\n";
    }
}

int main() {
    std::vector<Anime> animeList;
    const std::string nazwaPliku = "AnimeLista.docs";

    // Wczytanie istniejącej listy z pliku
    WczytajZPliku(nazwaPliku, animeList);

    char choice;
    do {
        Anime anime;
        std::cout << "Podaj tytuł anime: ";
        std::cin.ignore();
        std::getline(std::cin, anime.Nazwa);

        std::cout << "Podaj rok produkcji: ";
        std::cin >> anime.Rok_produkcji;

        std::cout << "Podaj liczbę odcinków: ";
        std::cin >> anime.Liczba_odcinkow;

        std::cout << "Czy planujesz rewatch? (1 - Tak, 0 - Nie): ";
        std::cin >> anime.Rewatch;
        std::cin.ignore(); // Czyszczenie bufora wejścia

        std::cout << "Podaj ścieżkę obrazu (lub wpisz 'E' aby wybrać plik): ";
        std::string sciezka;
        std::getline(std::cin, sciezka);

        if (sciezka == "E" || sciezka == "e") {
            sciezka = OtworzEksploratorPlikow();
            if (!sciezka.empty()) {
                anime.SciezkaObrazu = sciezka;
                std::cout << "Wybrano plik: " << sciezka << std::endl;
            } else {
                std::cout << "Nie wybrano pliku!\n";
            }
        } else {
            anime.SciezkaObrazu = sciezka;
        }

        animeList.push_back(anime);

        std::cout << "Czy chcesz dodać kolejne anime? (y/n): ";
        std::cin >> choice;
        std::cin.ignore(); // Czyszczenie bufora wejścia

    } while (choice == 'y' || choice == 'Y');

    // Zapis listy anime do pliku
    ZapiszDoPliku(nazwaPliku, animeList);

    std::cout << "Lista anime została zapisana do pliku " << nazwaPliku << "\n";

    return 0;
}