#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_FlightTrainerSim.h"

class FlightTrainerSim : public QMainWindow
{
    Q_OBJECT

public:
    FlightTrainerSim(QWidget *parent = nullptr);
    ~FlightTrainerSim();

private:
    Ui::FlightTrainerSimClass ui;
};

