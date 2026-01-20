#include "historia.h"

Historia::Historia(QObject *parent) : QObject(parent)
{
}

void Historia::dodajWpis(QString przedmiot, int blok, const QVector<int> &wylosowaneID)
{
    WpisHistorii nowyWpis;
    nowyWpis.czas = QDateTime::currentDateTime();
    nowyWpis.przedmiot = przedmiot;

    for(int id : wylosowaneID)
    {
        nowyWpis.pytania.push_back(SladPytania(blok, id));
    }

    m_rejestr.push_back(nowyWpis);
}

void Historia::oznaczJakoOdrzucone(QString przedmiot, int blok, int numerID)
{
    // Iterujemy od końca, aby znaleźć najnowszy egzamin/losowanie
    for(int i = m_rejestr.size() - 1; i >= 0; i--)
    {
        // Zabezpieczenie: sprawdzamy czy przedmiot się zgadza
        if(m_rejestr[i].przedmiot != przedmiot) continue;

        for(int j = 0; j < m_rejestr[i].pytania.size(); j++)
        {
            SladPytania &slad = m_rejestr[i].pytania[j];
            // Szukamy pasującego ID w danym bloku, które jeszcze nie jest odrzucone
            if(slad.blok == blok && slad.numerID == numerID && !slad.odrzucone)
            {
                slad.odrzucone = true;
                return; // Znaleziono i oznaczono, kończymy szukanie
            }
        }
    }
}

void Historia::wypiszHistorie()
{
    qDebug() << "=== PEŁNA HISTORIA SESJI ===";
    for(const auto &wpis : m_rejestr)
    {
        qDebug() << "Sesja:" << wpis.czas.toString("yyyy-MM-dd HH:mm:ss") << "|" << wpis.przedmiot;
        for(const auto &p : wpis.pytania)
        {
            QString stan = p.odrzucone ? "[ODRZUCONE]" : "[ZALICZONE]";
            qDebug() << "   Blok:" << p.blok << " ID:" << p.numerID << stan;
        }
    }
}

// Implementacja metody zapisującej
bool Historia::zapiszDoPliku(QString sciezka)
{
    QFile plik(sciezka);
    if (!plik.open(QIODevice::WriteOnly)) {
        qWarning() << "Nie można otworzyć pliku do zapisu:" << sciezka;
        return false;
    }

    QDataStream out(&plik);

    // Zapisujemy prostą sygnaturę (Magic Number), aby oznaczyć, że to nasz plik
    out << (quint32)0xAABBCCDD;

    // Zapisujemy cały wektor historii (Qt potrafi seryjnie zapisać QVector i zagnieżdżone struktury,
    // o ile zdefiniowaliśmy dla nich operatory << w pliku .h)
    out << m_rejestr;

    plik.close();
    qInfo() << "Zapisano historię do pliku:" << sciezka;
    return true;
}


bool Historia::wczytajZPliku(QString sciezka)
{
    QFile plik(sciezka);
    if (!plik.open(QIODevice::ReadOnly)) {
        qWarning() << "Nie można otworzyć pliku do odczytu:" << sciezka;
        return false;
    }

    QDataStream in(&plik);
    quint32 magic;
    in >> magic;

    if (magic != 0xAABBCCDD) {
        qWarning() << "Nieprawidłowy format pliku historii!";
        return false;
    }

    m_rejestr.clear();
    in >> m_rejestr;

    plik.close();
    return true;
}
