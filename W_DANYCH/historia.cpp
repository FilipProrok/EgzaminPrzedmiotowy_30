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
        // Tworzymy ślad pytania (domyślnie status: wylosowane/nieodrzucone)
        nowyWpis.pytania.push_back(SladPytania(blok, id));
    }

    m_rejestr.push_back(nowyWpis);

    // Logowanie dla potwierdzenia działania
    qInfo() << "HISTORIA: Dodano wpis dla" << przedmiot << "Blok:" << blok
            << "- liczba pytań:" << wylosowaneID.size();
}

void Historia::oznaczJakoOdrzucone(QString przedmiot, int blok, int numerID)
{
    // Szukamy od końca, aby znaleźć najświeższe wystąpienie tego pytania
    // (zakładamy, że odrzucamy to, co przed chwilą wylosowaliśmy)
    for(int i = m_rejestr.size() - 1; i >= 0; i--)
    {
        // Sprawdzamy czy zgadza się przedmiot (zabezpieczenie)
        if(m_rejestr[i].przedmiot != przedmiot) continue;

        for(int j = 0; j < m_rejestr[i].pytania.size(); j++)
        {
            SladPytania &slad = m_rejestr[i].pytania[j];
            if(slad.blok == blok && slad.numerID == numerID && !slad.odrzucone)
            {
                slad.odrzucone = true;
                qInfo() << "HISTORIA: Odrzucono pytanie ID:" << numerID << "w bloku" << blok;
                return; // Znaleziono i oznaczono, kończymy
            }
        }
    }
}

void Historia::wypiszHistorie()
{
    qDebug() << "=== PEŁNA HISTORIA EGZAMINÓW ===";
    for(const auto &wpis : m_rejestr)
    {
        qDebug() << wpis.czas.toString("HH:mm:ss") << "|" << wpis.przedmiot;
        for(const auto &p : wpis.pytania)
        {
            QString status = p.odrzucone ? "[ODRZUCONE]" : "[ZALICZONE]";
            qDebug() << "   Blok:" << p.blok << "ID:" << p.numerID << status;
        }
    }
}
