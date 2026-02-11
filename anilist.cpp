#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <windows.h> // Potrzebne do okienek Windowsa
#include <commdlg.h> // Potrzebne do okna wyboru pliku

// Magiczna linijka, żeby Visual Studio wiedziało skąd wziąć funkcje okienkowe
#pragma comment(lib, "Comdlg32.lib") 

class Anime {
public:
    int Data_produkcji;
    std::string Nazwa;
    int Liczba_odcinków;
    std::string Sciezka_do_obrazka; // Nowe pole

    // Funkcja otwierająca Eksplorator Plików
    std::string WybierzObrazek() {
        OPENFILENAMEA ofn;       // Struktura konfiguracji okna (wersja ANSI 'A')
        char szFile[260] = { 0 }; // Bufor na ścieżkę pliku

        // Inicjalizacja struktury zerami
        ZeroMemory(&ofn, sizeof(ofn));
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = NULL;
        ofn.lpstrFile = szFile;
        ofn.nMaxFile = sizeof(szFile);
        ofn.lpstrFilter = "Obrazki JPEG\0*.jpg;*.jpeg\0Wszystkie pliki\0*.*\0";
        ofn.nFilterIndex = 1;
        ofn.lpstrFileTitle = NULL;
        ofn.nMaxFileTitle = 0;
        ofn.lpstrInitialDir = NULL;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

        // Wyświetl okno. Jeśli użytkownik coś wybierze, zwróć ścieżkę.
        if (GetOpenFileNameA(&ofn) == TRUE) {
            return std::string(ofn.lpstrFile);
        }
        return ""; // Jeśli anulował
    }

    // Zapisujemy jako HTML, żeby Word to ładnie łyknął
    void ZapiszDoPliku(const std::string& nazwaPliku) {
        // Tryb append (dopisywanie), ale musimy uważać na strukturę HTML
        // Uproszczenie: po prostu dopisujemy fragmenty <div>
        std::ofstream plik(nazwaPliku, std::ios::app); 
        
        if (plik.is_open()) {
            // Zapisujemy w formacie HTML
            plik << "<div style='border-bottom: 2px solid black; padding: 10px;'>" << std::endl;
            plik << "<h2>Nazwa: " << Nazwa << "</h2>" << std::endl;
            plik << "<p><strong>Data produkcji:</strong> " << Data_produkcji << "</p>" << std::endl;
            plik << "<p><strong>Liczba odcinków:</strong> " << Liczba_odcinków << "</p>" << std::endl;
            
            if (!Sciezka_do_obrazka.empty()) {
                // Tutaj wstawiamy obrazek. Width ustawia szerokość, żeby nie był gigantyczny.
                plik << "<p><img src='" << Sciezka_do_obrazka << "' width='300'></p>" << std::endl;
            } else {
                plik << "<p><i>Brak obrazka</i></p>" << std::endl;
            }
            
            plik << "</div>" << std::endl;
            plik.close();
            std::cout << "[SUKCES] Zapisano dane do pliku!" << std::endl;
        } else {
            std::cerr << "[BLAD] Nie można otworzyć pliku!" << std::endl;
        }
    }
};

int main() {
    setlocale(LC_ALL, ""); 

    // Tworzymy nagłówek pliku HTML (jeśli plik nie istnieje)
    std::ifstream sprawdz("anime.html");
    if (!sprawdz.good()) {
        std::ofstream nowy("anime.html");
        nowy << "<html><head><meta charset='UTF-8'></head><body><h1>Moja Lista Anime</h1>" << std::endl;
        nowy.close();
    }
    sprawdz.close();

    while (true) {
        Anime anime;

        std::cout << "\n--- DODAWANIE NOWEGO ANIME ---\n";
        std::cout << "Podaj nazwę anime: ";
        // W poprzednim kodzie cin.ignore() było w złym miejscu jeśli pętla leci pierwszy raz
        // Ale tutaj zostawmy tak jak masz, tylko pamiętaj o czyszczeniu bufora
        std::string temp; 
        std::getline(std::cin, temp); // Czasem pierwszy getline łapie pusty znak po poprzednim cin
        if(temp.empty()) std::getline(std::cin, anime.Nazwa);
        else anime.Nazwa = temp;

        std::cout << "Podaj datę produkcji: ";
        std::cin >> anime.Data_produkcji;

        std::cout << "Podaj liczbę odcinków: ";
        std::cin >> anime.Liczba_odcinków;

        std::cout << "Czy chcesz dodać obrazek? (t/n): ";
        char czyObrazek;
        std::cin >> czyObrazek;
        
        if (czyObrazek == 't' || czyObrazek == 'T') {
            std::cout << "Otwieram okno wyboru..." << std::endl;
            anime.Sciezka_do_obrazka = anime.WybierzObrazek();
            if(anime.Sciezka_do_obrazka.empty()) {
                std::cout << "Nie wybrano pliku." << std::endl;
            } else {
                std::cout << "Wybrano: " << anime.Sciezka_do_obrazka << std::endl;
            }
        }

        // Zapisujemy do .html zamiast .txt
        anime.ZapiszDoPliku("anime.html");

        char kontynuuj;
        std::cout << "Czy chcesz dodać kolejne anime? (t/n): ";
        std::cin >> kontynuuj;

        if (kontynuuj != 't' && kontynuuj != 'T') break;
    }

    return 0;
}
