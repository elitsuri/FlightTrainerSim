// File: FlightControlPanel.h
#ifndef FLIGHTCONTROLPANEL_H
#define FLIGHTCONTROLPANEL_H

#include "IFlightModel.h"
#include <QWidget>
#include <QSlider>
#include <QPushButton>
#include <QLabel>
#include <QCheckBox>

class FlightControlPanel : public QWidget {
    Q_OBJECT
public:
    explicit FlightControlPanel(QWidget* parent = nullptr);
    ControlInputs getControls() const;
signals:
    void controlsChanged(const ControlInputs& controls);
private slots:
    void onControlChanged();
    void onAutopilotToggled(bool enabled);
private:
    QSlider *m_elevatorSlider, *m_aileronSlider, *m_rudderSlider, *m_throttleSlider, *m_flapsSlider;
    QCheckBox *m_gearCheckBox, *m_autopilotCheckBox;
    QLabel *m_elevatorLabel, *m_aileronLabel, *m_rudderLabel, *m_throttleLabel, *m_flapsLabel;
    bool m_autopilotEnabled;
    void setupUI();
    QSlider* createSlider(int min, int max, int value);
    void updateLabels();
};

#endif
