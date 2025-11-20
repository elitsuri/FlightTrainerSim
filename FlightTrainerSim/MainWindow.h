// File: MainWindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "SimulationEngine.h"
#include "Cockpit3DView.h"
#include "Outside3DView.h"
#include "FlightControlPanel.h"
#include "DebriefWindow.h"
#include <QMainWindow>
#include <QToolBar>
#include <QStatusBar>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QSplitter>
#include <memory>

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
private slots:
    void onStartClicked();
    void onPauseClicked();
    void onStopClicked();
    void onResetClicked();
    void onScenarioChanged(int index);
    void onAircraftChanged(int index);
    void onSimulationUpdated();
    void onScenarioFinished(bool success);
    void onWarningIssued(const QString& message);
    void onStateChanged(const QString& state);
    void onControlsChanged(const ControlInputs& controls);
private:
    std::unique_ptr<SimulationEngine> m_engine;
    std::unique_ptr<DebriefWindow> m_debriefWindow;
    QToolBar* m_toolbar;
    QStatusBar* m_statusBar;
    QLabel *m_statusLabel, *m_warningLabel;
    QPushButton *m_startButton, *m_pauseButton, *m_stopButton, *m_resetButton;
    QComboBox *m_scenarioCombo, *m_aircraftCombo;
    Cockpit3DView* m_cockpitView;
    Outside3DView* m_outsideView;
    FlightControlPanel* m_controlPanel;
    void setupUI();
    void setupToolbar();
    void setupCentralWidget();
    void setupStatusBar();
    void connectSignals();
    void initializeSimulation();
    void updateUI();
};

#endif
