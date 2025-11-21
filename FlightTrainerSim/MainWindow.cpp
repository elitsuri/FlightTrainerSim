// File: MainWindow.cpp - FIXED VERSION
#include "MainWindow.h"
#include "AircraftFactory.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), m_engine(std::make_unique<SimulationEngine>(this))
    , m_debriefWindow(std::make_unique<DebriefWindow>(this)) {
    setupUI();
    connectSignals();
    initializeSimulation();
    setWindowTitle("FlightTrainerSim - Professional Flight Training System");
    resize(1600, 1000);
}

void MainWindow::setupUI() {
    setupToolbar();
    setupCentralWidget();
    setupStatusBar();
}

void MainWindow::setupToolbar() {
    m_toolbar = addToolBar("Flight Controls");
    m_toolbar->setMovable(false);

    // FIX 1: Better button styling
    m_startButton = new QPushButton("▶ Start");
    m_startButton->setStyleSheet("QPushButton { background-color: #1a5490; color: white; font-weight: bold; padding: 8px; }");
    m_toolbar->addWidget(m_startButton);

    m_pauseButton = new QPushButton("⏸ Pause");
    m_pauseButton->setEnabled(false);
    m_pauseButton->setStyleSheet("QPushButton { background-color: #1a5490; color: white; font-weight: bold; padding: 8px; }");
    m_toolbar->addWidget(m_pauseButton);

    m_stopButton = new QPushButton("⏹ Stop");
    m_stopButton->setEnabled(false);
    m_stopButton->setStyleSheet("QPushButton { background-color: #1a5490; color: white; font-weight: bold; padding: 8px; }");
    m_toolbar->addWidget(m_stopButton);

    m_resetButton = new QPushButton("↻ Reset");
    m_resetButton->setStyleSheet("QPushButton { background-color: #1a5490; color: white; font-weight: bold; padding: 8px; }");
    m_toolbar->addWidget(m_resetButton);

    m_toolbar->addSeparator();
    m_toolbar->addWidget(new QLabel(" Scenario: "));

    m_scenarioCombo = new QComboBox();
    m_scenarioCombo->addItem("Basic Takeoff");
    m_scenarioCombo->addItem("Traffic Pattern");
    m_scenarioCombo->addItem("IFR Basic Navigation");
    m_scenarioCombo->addItem("Engine Failure Recovery");
    m_toolbar->addWidget(m_scenarioCombo);

    m_toolbar->addSeparator();
    m_toolbar->addWidget(new QLabel(" Aircraft: "));

    m_aircraftCombo = new QComboBox();
    m_aircraftCombo->addItem("T-38 Trainer");
    m_aircraftCombo->addItem("F-16 Fighter");
    m_aircraftCombo->addItem("C-130 Cargo");
    m_toolbar->addWidget(m_aircraftCombo);
}

void MainWindow::setupCentralWidget() {
    auto* centralWidget = new QWidget();
    setCentralWidget(centralWidget);
    auto* mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    auto* viewsSplitter = new QSplitter(Qt::Vertical);
    m_cockpitView = new Cockpit3DView();
    viewsSplitter->addWidget(m_cockpitView);
    m_outsideView = new Outside3DView();
    viewsSplitter->addWidget(m_outsideView);
    viewsSplitter->setSizes({ 500, 500 });

    m_controlPanel = new FlightControlPanel();
    m_controlPanel->setMaximumWidth(350);

    mainLayout->addWidget(viewsSplitter, 1);
    mainLayout->addWidget(m_controlPanel);
}

void MainWindow::setupStatusBar() {
    m_statusBar = statusBar();
    m_statusLabel = new QLabel("Ready - Select aircraft and scenario, then click Start");
    m_statusBar->addWidget(m_statusLabel);
    m_statusBar->addPermanentWidget(new QLabel(" | "));
    m_warningLabel = new QLabel("");
    m_warningLabel->setStyleSheet("color: #ff0; font-weight: bold;");
    m_statusBar->addPermanentWidget(m_warningLabel);
}

void MainWindow::connectSignals() {
    connect(m_startButton, &QPushButton::clicked, this, &MainWindow::onStartClicked);
    connect(m_pauseButton, &QPushButton::clicked, this, &MainWindow::onPauseClicked);
    connect(m_stopButton, &QPushButton::clicked, this, &MainWindow::onStopClicked);
    connect(m_resetButton, &QPushButton::clicked, this, &MainWindow::onResetClicked);
    connect(m_scenarioCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
        this, &MainWindow::onScenarioChanged);
    connect(m_aircraftCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
        this, &MainWindow::onAircraftChanged);
    connect(m_engine.get(), &SimulationEngine::simulationUpdated, this, &MainWindow::onSimulationUpdated);
    connect(m_engine.get(), &SimulationEngine::scenarioFinished, this, &MainWindow::onScenarioFinished);
    connect(m_engine.get(), &SimulationEngine::warningIssued, this, &MainWindow::onWarningIssued);
    connect(m_engine.get(), &SimulationEngine::stateChanged, this, &MainWindow::onStateChanged);
    connect(m_controlPanel, &FlightControlPanel::controlsChanged, this, &MainWindow::onControlsChanged);
}

void MainWindow::initializeSimulation() {
    onAircraftChanged(0);
    onScenarioChanged(0);
}

// FIX 2: Better Start button handling
void MainWindow::onStartClicked() {
    m_engine->start();
    updateUI();  // Update button states immediately!
    m_statusLabel->setText("Status: Running - Use controls to fly!");
}

void MainWindow::onPauseClicked() {
    m_engine->pause();
    m_pauseButton->setText(m_engine->isPaused() ? "▶ Resume" : "⏸ Pause");
    m_statusLabel->setText(m_engine->isPaused() ? "Status: Paused" : "Status: Running");
}

void MainWindow::onStopClicked() {
    m_engine->stop();
    updateUI();
    m_statusLabel->setText("Status: Stopped");
}

void MainWindow::onResetClicked() {
    m_engine->reset();
    m_warningLabel->clear();
    updateUI();
    m_statusLabel->setText("Status: Reset - Ready to start");
}

void MainWindow::onScenarioChanged(int index) {
    std::unique_ptr<TrainingScenario> scenario;
    switch (index) {
    case 0: scenario = TrainingScenario::createBasicTakeoffScenario(); break;
    case 1: scenario = TrainingScenario::createPatternScenario(); break;
    case 2: scenario = TrainingScenario::createIFRBasicScenario(); break;
    case 3: scenario = TrainingScenario::createEngineFailureScenario(); break;
    default: scenario = TrainingScenario::createBasicTakeoffScenario();
    }
    m_engine->setScenario(std::move(scenario));
}

void MainWindow::onAircraftChanged(int index) {
    AircraftType type;
    switch (index) {
    case 0: type = AircraftType::Trainer; break;
    case 1: type = AircraftType::Jet; break;
    case 2: type = AircraftType::Cargo; break;
    default: type = AircraftType::Trainer;
    }
    auto aircraft = AircraftFactory::createAircraft(type);
    m_engine->setActiveAircraft(std::move(aircraft));
    m_cockpitView->setAircraft(m_engine->activeAircraft());
    m_outsideView->setAircraft(m_engine->activeAircraft());
    m_outsideView->setEnvironment(m_engine->environment());
}

void MainWindow::onSimulationUpdated() {
    m_cockpitView->update();
    m_outsideView->update();
}

void MainWindow::onScenarioFinished(bool success) {
    updateUI();
    if (success) {
        m_statusLabel->setText("Scenario Completed Successfully!");
        DebriefReport report;
        report.generate(*m_engine->metrics(), *m_engine->scenario(), *m_engine->activeAircraft());
        m_debriefWindow->setReport(report, *m_engine->metrics());
        m_debriefWindow->exec();
    }
    else {
        m_statusLabel->setText("Scenario Failed");
        QMessageBox::warning(this, "Scenario Failed",
            "The scenario was not completed successfully. Try again!");
    }
}

void MainWindow::onWarningIssued(const QString& message) {
    m_warningLabel->setText("⚠ " + message);
}

void MainWindow::onStateChanged(const QString& state) {
    updateUI();
}

// FIX 3: Always update controls
void MainWindow::onControlsChanged(const ControlInputs& controls) {
    m_engine->setControlInputs(controls);
}

// FIX 4: Proper button state management
void MainWindow::updateUI() {
    bool isRunning = m_engine->isRunning();
    bool isPaused = m_engine->isPaused();

    m_startButton->setEnabled(!isRunning);
    m_pauseButton->setEnabled(isRunning);
    m_stopButton->setEnabled(isRunning);
    m_scenarioCombo->setEnabled(!isRunning);
    m_aircraftCombo->setEnabled(!isRunning);

    m_pauseButton->setText(isPaused ? "▶ Resume" : "⏸ Pause");
}