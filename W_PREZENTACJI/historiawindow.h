#ifndef HISTORIAWINDOW_H
#define HISTORIAWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QHeaderView>
#include "W_DANYCH/historia.h"
#include "W_USLUG/program.h"

class HistoriaWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit HistoriaWindow(const Historia &historia, Program *program = nullptr, QWidget *parent = nullptr);
    ~HistoriaWindow();

private:
    // Zamiast "Ui::HistoriaWindow *ui" mamy własne wskaźniki na elementy GUI
    QTableWidget *m_tabela;

    // Dane
    QVector<WpisHistorii> m_rejestrDanych;
    Program *m_programRef;

    // Metody pomocnicze
    void konfigurujTabele();
    void wypelnijDane();
};

#endif // HISTORIAWINDOW_H
