#ifndef HISTORIA_H
#define HISTORIA_H

#include <QObject>
#include <QVector>
#include <QDateTime>
#include <QDebug>
#include <QDataStream>
#include <QFile>

// Struktura przechowująca minimalny ślad pytania (bez treści)
// Zawiera operatory << i >> umożliwiające zapis binarny
struct SladPytania {
    int blok;       // Numer bloku (np. 0 lub 1)
    int numerID;    // Indeks pytania w bazie
    bool odrzucone; // Flaga stanu

    SladPytania() = default; // Konstruktor domyślny wymagany przy deserializacji
    SladPytania(int b, int id) : blok(b), numerID(id), odrzucone(false) {}

    // Operator zapisu do strumienia binarnego
    friend QDataStream &operator<<(QDataStream &out, const SladPytania &slad) {
        out << slad.blok << slad.numerID << slad.odrzucone;
        return out;
    }

    // Operator odczytu ze strumienia binarnego
    friend QDataStream &operator>>(QDataStream &in, SladPytania &slad) {
        in >> slad.blok >> slad.numerID >> slad.odrzucone;
        return in;
    }
};

// Struktura pojedynczego wpisu w historii (jeden rzut losowania)
struct WpisHistorii {
    QDateTime czas;
    QString przedmiot;
    QVector<SladPytania> pytania;

    // Operator zapisu
    friend QDataStream &operator<<(QDataStream &out, const WpisHistorii &wpis) {
        out << wpis.czas << wpis.przedmiot << wpis.pytania;
        return out;
    }

    // Operator odczytu
    friend QDataStream &operator>>(QDataStream &in, WpisHistorii &wpis) {
        in >> wpis.czas >> wpis.przedmiot >> wpis.pytania;
        return in;
    }
};

class Historia : public QObject
{
    Q_OBJECT
private:
    QVector<WpisHistorii> m_rejestr;

public:
    explicit Historia(QObject *parent = nullptr);

    // Rejestruje partię wylosowanych pytań
    void dodajWpis(QString przedmiot, int blok, const QVector<int>& wylosowaneID);

    // Oznacza konkretne pytanie jako odrzucone (szuka od końca historii)
    void oznaczJakoOdrzucone(QString przedmiot, int blok, int numerID);

    // Metoda pomocnicza do debugowania
    void wypiszHistorie();

    bool zapiszDoPliku(QString sciezka);

    bool wczytajZPliku(QString sciezka);

    const QVector<WpisHistorii>& getRejestr() const { return m_rejestr; }
};

#endif // HISTORIA_H
