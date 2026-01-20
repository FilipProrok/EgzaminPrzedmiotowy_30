#include "historiawindow.h"
#include <QDate>
#include <QBrush>
#include <QColor>

HistoriaWindow::HistoriaWindow(const Historia &historia, Program *program, QWidget *parent) :
    QMainWindow(parent),
    m_rejestrDanych(historia.getRejestr()),
    m_programRef(program)
{
    // 1. Ustawienie głównego widgetu i tytułu okna
    QWidget *centralWidget = new QWidget(this);
    this->setCentralWidget(centralWidget);
    this->setWindowTitle("Pełna Historia Egzaminów");
    this->resize(900, 600); // Domyślny rozmiar

    // 2. Tworzenie Layoutu (układu pionowego)
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    // 3. Tworzenie Tabeli
    m_tabela = new QTableWidget(this);
    layout->addWidget(m_tabela); // Dodajemy tabelę do układu

    // 4. Konfiguracja i wypełnienie
    konfigurujTabele();
    wypelnijDane();
}

HistoriaWindow::~HistoriaWindow()
{
    // W Qt obiekty (jak m_tabela) będące dziećmi (children) są usuwane automatycznie
    // wraz z rodzicem, więc nie musimy ręcznie robić delete m_tabela.
}

void HistoriaWindow::konfigurujTabele()
{
    // Ustawiamy 5 kolumn zgodnie z Twoim życzeniem
    m_tabela->setColumnCount(5);

    QStringList naglowki;
    naglowki << "Data" << "Przedmiot" << "Blok" << "Pytanie" << "Status";
    m_tabela->setHorizontalHeaderLabels(naglowki);

    // Stylizacja tabeli
    m_tabela->setAlternatingRowColors(true); // Paski naprzemienne dla czytelności
    m_tabela->setSelectionBehavior(QAbstractItemView::SelectRows); // Zaznaczanie całego wiersza
    m_tabela->setEditTriggers(QAbstractItemView::NoEditTriggers); // Blokada edycji

    // Rozciąganie kolumny z treścią pytania, żeby zajęła resztę miejsca
    m_tabela->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents); // Data
    m_tabela->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents); // Przedmiot
    m_tabela->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Fixed);            // Blok
    m_tabela->setColumnWidth(2, 50);
    m_tabela->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);          // Pytanie (rozciągnij)
    m_tabela->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents); // Status
}

void HistoriaWindow::wypelnijDane()
{
    // Czyścimy tabelę na start (ustawia licznik wierszy na 0)
    m_tabela->setRowCount(0);

    // Iterujemy przez sesje (egzaminy)
    for(const auto &sesja : m_rejestrDanych)
    {
        // Sprawdzamy, czy dla tej sesji możemy pobrać treści pytań
        // (tylko jeśli w programie wczytany jest ten sam przedmiot)
        bool trescDostepna = (m_programRef && m_programRef->getAktPrzedmiot() == sesja.przedmiot);

        // Iterujemy przez pytania w danej sesji
        for(const auto &slad : sesja.pytania)
        {
            // Dodajemy nowy wiersz na końcu tabeli
            int row = m_tabela->rowCount();
            m_tabela->insertRow(row);

            // 1. Data
            QTableWidgetItem *itemData = new QTableWidgetItem(sesja.czas.toString("yyyy-MM-dd HH:mm"));
            m_tabela->setItem(row, 0, itemData);

            // 2. Przedmiot
            m_tabela->setItem(row, 1, new QTableWidgetItem(sesja.przedmiot));

            // 3. Blok (konwersja 0 -> A, 1 -> B)
            QString blokStr = (slad.blok == 0) ? "A" : "B";
            QTableWidgetItem *itemBlok = new QTableWidgetItem(blokStr);
            itemBlok->setTextAlignment(Qt::AlignCenter);
            m_tabela->setItem(row, 2, itemBlok);

            // 4. Pytanie (Treść lub ID)
            QString tekstPytania;
            if (trescDostepna) {
                // Pobieramy treść z Programu
                tekstPytania = m_programRef->pobierzTrescPytania(slad.blok, slad.numerID);
            } else {
                // Brak dostępu do bazy tego przedmiotu -> pokazujemy ID
                tekstPytania = QString("ID: %1 (Brak treści - inny przedmiot)").arg(slad.numerID);
            }
            m_tabela->setItem(row, 3, new QTableWidgetItem(tekstPytania));

            // 5. Status (Kolorowanie)
            QString statusStr = slad.odrzucone ? "ODRZUCONE" : "ZDANE";
            QTableWidgetItem *itemStatus = new QTableWidgetItem(statusStr);

            if (slad.odrzucone) {
                itemStatus->setForeground(QBrush(Qt::red));
                itemStatus->setFont(QFont("Arial", 9, QFont::Bold));
            } else {
                itemStatus->setForeground(QBrush(Qt::darkGreen));
            }
            m_tabela->setItem(row, 4, itemStatus);
        }
    }
}
