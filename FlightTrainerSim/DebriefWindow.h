// File: DebriefWindow.h
#ifndef DEBRIEFWINDOW_H
#define DEBRIEFWINDOW_H

#include "FlightMetrics.h"
#include <QDialog>
#include <QTextEdit>
#include <QTableWidget>
#include <QPushButton>

class DebriefWindow : public QDialog {
    Q_OBJECT
public:
    explicit DebriefWindow(QWidget* parent = nullptr);
    void setReport(const DebriefReport& report, const FlightMetrics& metrics);
private:
    QTextEdit* m_summaryText;
    QTableWidget* m_scoresTable;
    QPushButton* m_closeButton;
    void setupUI();
};

#endif
