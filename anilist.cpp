#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <windows.h> 
#include <commdlg.h> 
#include <codecvt> // Potrzebne do konwersji Unicode -> UTF-8

// Linkowanie biblioteki systemowej dla okienek 
#pragma comment(lib, "Comdlg32.lib") 

// Pomocnicza funkcja: Konwertuje wstring (Unicode) na string (UTF-8) dla pliku HTML
std::string WideToUTF8(const std::wstring& wstr) {
    if (wstr.empty()) return "";
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
    std::string strTo(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
    return strTo;
}

class Anime {
public:
    int Data_produkcji;
    std::wstring Nazwa; // Zmiana na wstring dla pełnego Unicode 
    int Liczba_odcinków;
    std::wstring Sciezka_do_obrazka; 

    // Otwiera okno wyboru pliku w wersji Unicode 
    std::wstring WybierzObrazek() {
        OPENFILENAMEW ofn; // Wersja 'W' (Wide) dla Unicode
        wchar_t szFile[260] = { 0 };

        ZeroMemory(&ofn, sizeof(ofn));
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = NULL;
        ofn.lpstrFile = szFile;
        ofn.nMaxFile = sizeof(szFile);
        ofn.lpstrFilter = L"Obrazki\0*.jpg;*.jpeg;*.png\0Wszystkie pliki\0*.*\0";
        ofn.nFilterIndex = 1;
        // Kluczowa flaga: OFN_NOCHANGEDIR - zapobiega ucieczce programu z folderu roboczego
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

        if (GetOpenFileNameW(&ofn) == TRUE) {
            std::wstring path = ofn.lpstrFile;
            // Konwersja ścieżki Windows (\) na format webowy (/)
            for (size_t i = 0; i < path.length(); ++i) {
                if (path[i] == L'\\') path[i] = L'/';
            }
            return path;
        }
        return L"";
    }

    // Zapisuje dane jako blok HTML 
    void ZapiszDoPliku(const std::string& nazwaPliku) {
        std::ofstream plik(nazwaPliku, std::ios::app); 
        
        if (plik.is_open()) {
            plik << "<div style='border-bottom: 2px solid #333; padding: 20px; margin-bottom: 10px; font-family: sans-serif;'>" << std::endl;
            plik << "<h2>Nazwa: " << WideToUTF8(Nazwa) << "</h2>" << std::endl;
            plik << "<p><strong>Data produkcji:</strong> " << Data_produkcji << "</p>" << std::endl;
            plik << "<p><strong>Liczba odcinków:</strong> " << Liczba_odcinków << "</p>" << std::endl;
            
            if (!Sciezka_do_obrazka.empty()) {
                // file:/// zapewnia, że przeglądarka poprawnie wczyta plik lokalny
                plik << "<p><img src='file:///" << WideToUTF8(Sciezka_do_obrazka) << "' width='400' style='border-radius: 8px;'></p>" << std::endl;
            } else {
                plik << "<p style='color: gray;'><i>Brak obrazka</i></p>" << std::endl;
            }
            
            plik << "</div>" << std::endl;
            plik.close();
            std::wcout << L"[SUKCES] Dodano: " << Nazwa << std::endl;
        } else {
            std::cerr << "[BLAD] Nie mozna otworzyc pliku anime.html!" << std::endl;
        }
    }
};

int main() {
    // Ustawienie polskiej lokalizacji dla konsoli
    std::wcout.imbue(std::locale(""));
    std::wcin.imbue(std::locale(""));

    // Inicjalizacja pliku HTML 
    std::ifstream sprawdz("anime.html");
    if (!sprawdz.good()) {
        std::ofstream nowy("anime.html");
        // UTF-8 w meta tagu zapewnia poprawne wyświetlanie polskich znaków w przeglądarce 
        nowy << "<html><head><meta charset='UTF-8'><title>Moja Lista Anime</title></head><body style='background: #f4f4f4;'>" << std::endl;
        nowy << "<h1 style='text-align: center;'>Kolekcja Anime</h1>" << std::endl;
        nowy.close();
    }
    sprawdz.close();

    while (true) {
        Anime anime;

        std::wcout << L"\n--- NOWA POZYCJA ---\n";
        std::wcout << L"Nazwa anime: ";
        // std::ws czyści bufor z poprzednich znaków nowej linii 
        std::getline(std::wcin >> std::ws, anime.Nazwa);

        std::wcout << L"Data produkcji (rok): ";
        std::wcin >> anime.Data_produkcji;

        std::wcout << L"Liczba odcinków: ";
        std::wcin >> anime.Liczba_odcinków;

        std::wcout << L"Czy chcesz dodac obrazek? (t/n): ";
        wchar_t czyObrazek;
        std::wcin >> czyObrazek;
        
        if (czyObrazek == L't' || czyObrazek == L'T') {
            std::wcout << L"Wybierz plik w okienku..." << std::endl;
            anime.Sciezka_do_obrazka = anime.WybierzObrazek();
        }

        anime.ZapiszDoPliku("anime.html");

        std::wcout << L"Dodac kolejne? (t/n): ";
        std::wcin >> czyObrazek;
        if (czyObrazek != L't' && czyObrazek != L'T') break;
    }

    return 0;
}
