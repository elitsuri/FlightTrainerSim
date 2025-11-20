// File: Cockpit3DView.h
#ifndef COCKPIT3DVIEW_H
#define COCKPIT3DVIEW_H

#include "Aircraft.h"
#include <QWidget>
#include <QPainter>

class Cockpit3DView : public QWidget {
    Q_OBJECT
public:
    explicit Cockpit3DView(QWidget* parent = nullptr);
    void setAircraft(Aircraft* aircraft) { m_aircraft = aircraft; }
protected:
    void paintEvent(QPaintEvent* event) override;
private:
    Aircraft* m_aircraft;
    void drawHUD(QPainter& painter);
    void drawAttitudeIndicator(QPainter& painter, int cx, int cy, int size);
    void drawAirspeedIndicator(QPainter& painter, int x, int y, int width, int height);
    void drawAltimeter(QPainter& painter, int x, int y, int width, int height);
    void drawHeadingIndicator(QPainter& painter, int x, int y, int width);
    void drawVerticalSpeed(QPainter& painter, int x, int y, int width, int height);
    void drawThrottleGauge(QPainter& painter, int x, int y, int width, int height);
    void drawWarnings(QPainter& painter);
    void drawCrosshair(QPainter& painter, int cx, int cy);
    QColor hudPrimaryColor() const { return QColor(0, 255, 0); }
    QColor hudSecondaryColor() const { return QColor(255, 255, 255); }
    QColor hudWarningColor() const { return QColor(255, 170, 0); }
    QColor hudCriticalColor() const { return QColor(255, 0, 0); }
};

#endif
