#ifndef HISTORIA_H
#define HISTORIA_H

#include <QObject>
#include <QVector>
#include <QDateTime>
#include <QDebug>

// Struktura przechowująca minimalne dane potrzebne do namierzenia pytania w bazie
struct SladPytania {
    int blok;       // Numer bloku (np. 0 dla A, 1 dla B)
    int numerID;    // Unikalny numer pytania wewnątrz bloku (z pliku)
    bool odrzucone; // Flaga: false = wylosowane, true = odrzucone przez użytkownika

    SladPytania(int b, int id) : blok(b), numerID(id), odrzucone(false) {}
};

// Struktura reprezentująca pojedynczy rzut (losowanie) w ramach egzaminu
struct WpisHistorii {
    QDateTime czas;
    QString przedmiot;
    QVector<SladPytania> pytania;
};

class Historia : public QObject
{
    Q_OBJECT
private:
    // Lista wszystkich operacji losowania od uruchomienia programu
    QVector<WpisHistorii> m_rejestr;

public:
    explicit Historia(QObject *parent = nullptr);

    // Rejestruje nowe wylosowane pytania
    void dodajWpis(QString przedmiot, int blok, const QVector<int>& wylosowaneID);

    // Oznacza konkretne pytania jako odrzucone w ostatnim wpisie
    // (lub wyszukuje odpowiedni wpis, jeśli obsługa jest bardziej złożona)
    void oznaczJakoOdrzucone(QString przedmiot, int blok, int numerID);

    // Metoda pomocnicza do podglądu historii (np. w konsoli debug)
    void wypiszHistorie();
};

#endif // HISTORIA_H
