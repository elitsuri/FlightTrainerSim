// File: DebriefWindow.cpp
#include "DebriefWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QHeaderView>
#include <QFont>

DebriefWindow::DebriefWindow(QWidget* parent) : QDialog(parent) {
    setupUI();
    resize(800, 600);
    setWindowTitle("Flight Training Debrief");
}

void DebriefWindow::setupUI() {
    auto* mainLayout = new QVBoxLayout(this);
    auto* titleLabel = new QLabel("FLIGHT TRAINING DEBRIEF");
    QFont titleFont("Arial", 16, QFont::Bold);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("background-color: #1a5490; color: white; padding: 10px;");
    mainLayout->addWidget(titleLabel);
    m_summaryText = new QTextEdit();
    m_summaryText->setReadOnly(true);
    m_summaryText->setFont(QFont("Courier", 10));
    mainLayout->addWidget(m_summaryText);
    m_scoresTable = new QTableWidget();
    m_scoresTable->setColumnCount(2);
    m_scoresTable->setHorizontalHeaderLabels({"Category", "Score"});
    m_scoresTable->horizontalHeader()->setStretchLastSection(true);
    m_scoresTable->verticalHeader()->setVisible(false);
    m_scoresTable->setAlternatingRowColors(true);
    m_scoresTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mainLayout->addWidget(m_scoresTable);
    auto* buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    m_closeButton = new QPushButton("Close");
    m_closeButton->setMinimumWidth(100);
    connect(m_closeButton, &QPushButton::clicked, this, &QDialog::accept);
    buttonLayout->addWidget(m_closeButton);
    mainLayout->addLayout(buttonLayout);
}

void DebriefWindow::setReport(const DebriefReport& report, const FlightMetrics& metrics) {
    m_summaryText->setPlainText(QString::fromStdString(report.summaryText()));
    QString recText = "\n\nRecommendations:\n";
    for (const auto& rec : report.recommendations()) {
        recText += "• " + QString::fromStdString(rec) + "\n";
    }
    m_summaryText->append(recText);
    m_scoresTable->setRowCount(static_cast<int>(report.categoryScores().size()));
    int row = 0;
    for (const auto& [category, score] : report.categoryScores()) {
        auto* categoryItem = new QTableWidgetItem(QString::fromStdString(category));
        auto* scoreItem = new QTableWidgetItem(QString("%1/100").arg(static_cast<int>(score)));
        QColor scoreColor;
        if (score >= 90) scoreColor = QColor(0, 200, 0);
        else if (score >= 70) scoreColor = QColor(200, 200, 0);
        else scoreColor = QColor(200, 0, 0);
        scoreItem->setForeground(scoreColor);
        scoreItem->setFont(QFont("Arial", 11, QFont::Bold));
        m_scoresTable->setItem(row, 0, categoryItem);
        m_scoresTable->setItem(row, 1, scoreItem);
        row++;
    }
    m_scoresTable->resizeColumnsToContents();
}
