Lokalny Tracker Anime (C++ / HTML Generator)
Opis projektu
Aplikacja konsolowa napisana w C++, służąca do zarządzania osobistą bazą obejrzanych serii anime (klon funkcjonalności AniList działający offline). Program zbiera dane od użytkownika i dynamicznie generuje sformatowany, statyczny plik HTML (anime.html), oddzielając warstwę logiki aplikacji (C++) od warstwy prezentacji danych (Przeglądarka internetowa).
Kluczowe rozwiązania inżynieryjne:
Natywne integracje OS: Wykorzystanie Windows API (GetOpenFileNameW z biblioteki <commdlg.h>) do obsługi systemowego okna dialogowego wyboru obrazków. Zapobiega to błędom przy ręcznym wprowadzaniu ścieżek do plików.
Zarządzanie kodowaniem: Autorska implementacja konwersji ciągów znaków (Unicode do UTF-8 za pomocą WideCharToMultiByte), co zapewnia poprawne renderowanie polskich znaków diakrytycznych w przeglądarce bez użycia ciężkich bibliotek zewnętrznych.
System Wejścia/Wyjścia (I/O): Ciągły nasłuch w pętli while pozwalający na seryjne dodawanie obiektów klasy Anime do pliku wyjściowego przy pomocy strumieni std::ofstream.
Technologie:
C++ (Standard I/O, File Streams, Object-Oriented Programming)
Windows API
HTML/CSS (Generowany dynamicznie)