// File: FlightControlPanel.cpp
#include "FlightControlPanel.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>

FlightControlPanel::FlightControlPanel(QWidget* parent) : QWidget(parent), m_autopilotEnabled(false) {
    setupUI();
}

void FlightControlPanel::setupUI() {
    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(10);
    auto* primaryGroup = new QGroupBox("Primary Flight Controls");
    auto* primaryLayout = new QVBoxLayout(primaryGroup);
    auto* elevatorLayout = new QHBoxLayout();
    m_elevatorLabel = new QLabel("Elevator: 0%");
    m_elevatorLabel->setMinimumWidth(120);
    m_elevatorSlider = createSlider(-100, 100, 0);
    elevatorLayout->addWidget(m_elevatorLabel);
    elevatorLayout->addWidget(m_elevatorSlider);
    primaryLayout->addLayout(elevatorLayout);
    auto* aileronLayout = new QHBoxLayout();
    m_aileronLabel = new QLabel("Aileron: 0%");
    m_aileronLabel->setMinimumWidth(120);
    m_aileronSlider = createSlider(-100, 100, 0);
    aileronLayout->addWidget(m_aileronLabel);
    aileronLayout->addWidget(m_aileronSlider);
    primaryLayout->addLayout(aileronLayout);
    auto* rudderLayout = new QHBoxLayout();
    m_rudderLabel = new QLabel("Rudder: 0%");
    m_rudderLabel->setMinimumWidth(120);
    m_rudderSlider = createSlider(-100, 100, 0);
    rudderLayout->addWidget(m_rudderLabel);
    rudderLayout->addWidget(m_rudderSlider);
    primaryLayout->addLayout(rudderLayout);
    mainLayout->addWidget(primaryGroup);
    auto* secondaryGroup = new QGroupBox("Secondary Controls");
    auto* secondaryLayout = new QVBoxLayout(secondaryGroup);
    auto* throttleLayout = new QHBoxLayout();
    m_throttleLabel = new QLabel("Throttle: 0%");
    m_throttleLabel->setMinimumWidth(120);
    m_throttleSlider = createSlider(0, 100, 0);
    throttleLayout->addWidget(m_throttleLabel);
    throttleLayout->addWidget(m_throttleSlider);
    secondaryLayout->addLayout(throttleLayout);
    auto* flapsLayout = new QHBoxLayout();
    m_flapsLabel = new QLabel("Flaps: 0%");
    m_flapsLabel->setMinimumWidth(120);
    m_flapsSlider = createSlider(0, 100, 0);
    flapsLayout->addWidget(m_flapsLabel);
    flapsLayout->addWidget(m_flapsSlider);
    secondaryLayout->addLayout(flapsLayout);
    mainLayout->addWidget(secondaryGroup);
    auto* systemGroup = new QGroupBox("Systems");
    auto* systemLayout = new QVBoxLayout(systemGroup);
    m_gearCheckBox = new QCheckBox("Landing Gear Down");
    m_gearCheckBox->setChecked(true);
    systemLayout->addWidget(m_gearCheckBox);
    m_autopilotCheckBox = new QCheckBox("Autopilot");
    m_autopilotCheckBox->setChecked(false);
    systemLayout->addWidget(m_autopilotCheckBox);
    mainLayout->addWidget(systemGroup);
    mainLayout->addStretch();
    connect(m_elevatorSlider, &QSlider::valueChanged, this, &FlightControlPanel::onControlChanged);
    connect(m_aileronSlider, &QSlider::valueChanged, this, &FlightControlPanel::onControlChanged);
    connect(m_rudderSlider, &QSlider::valueChanged, this, &FlightControlPanel::onControlChanged);
    connect(m_throttleSlider, &QSlider::valueChanged, this, &FlightControlPanel::onControlChanged);
    connect(m_flapsSlider, &QSlider::valueChanged, this, &FlightControlPanel::onControlChanged);
    connect(m_gearCheckBox, &QCheckBox::toggled, this, &FlightControlPanel::onControlChanged);
    connect(m_autopilotCheckBox, &QCheckBox::toggled, this, &FlightControlPanel::onAutopilotToggled);
}

QSlider* FlightControlPanel::createSlider(int min, int max, int value) {
    auto* slider = new QSlider(Qt::Horizontal);
    slider->setMinimum(min);
    slider->setMaximum(max);
    slider->setValue(value);
    return slider;
}

void FlightControlPanel::onControlChanged() {
    updateLabels();
    emit controlsChanged(getControls());
}

void FlightControlPanel::onAutopilotToggled(bool enabled) {
    m_autopilotEnabled = enabled;
    m_elevatorSlider->setEnabled(!enabled);
    m_aileronSlider->setEnabled(!enabled);
    m_rudderSlider->setEnabled(!enabled);
    if (enabled) {
        m_elevatorSlider->setValue(0);
        m_aileronSlider->setValue(0);
        m_rudderSlider->setValue(0);
    }
    onControlChanged();
}

void FlightControlPanel::updateLabels() {
    m_elevatorLabel->setText(QString("Elevator: %1%").arg(m_elevatorSlider->value()));
    m_aileronLabel->setText(QString("Aileron: %1%").arg(m_aileronSlider->value()));
    m_rudderLabel->setText(QString("Rudder: %1%").arg(m_rudderSlider->value()));
    m_throttleLabel->setText(QString("Throttle: %1%").arg(m_throttleSlider->value()));
    m_flapsLabel->setText(QString("Flaps: %1%").arg(m_flapsSlider->value()));
}

ControlInputs FlightControlPanel::getControls() const {
    ControlInputs controls;
    controls.elevator = m_elevatorSlider->value() / 100.0;
    controls.aileron = m_aileronSlider->value() / 100.0;
    controls.rudder = m_rudderSlider->value() / 100.0;
    controls.throttle = m_throttleSlider->value() / 100.0;
    controls.flaps = m_flapsSlider->value() / 100.0;
    controls.gearDown = m_gearCheckBox->isChecked();
    return controls;
}
