// File: Outside3DView.h
#ifndef OUTSIDE3DVIEW_H
#define OUTSIDE3DVIEW_H

#include "Aircraft.h"
#include "Environment.h"
#include <QWidget>
#include <QPainter>

class Outside3DView : public QWidget {
    Q_OBJECT
public:
    explicit Outside3DView(QWidget* parent = nullptr);
    void setAircraft(Aircraft* aircraft) { m_aircraft = aircraft; }
    void setEnvironment(Environment* env) { m_environment = env; }
protected:
    void paintEvent(QPaintEvent* event) override;
private:
    Aircraft* m_aircraft;
    Environment* m_environment;
    double m_cameraDistance, m_cameraAngle;
    void drawScene(QPainter& painter);
    void drawSkyAndGround(QPainter& painter);
    void drawAircraft(QPainter& painter, int cx, int cy);
    void drawFlightPath(QPainter& painter);
    void drawWaypoints(QPainter& painter);
    void drawRunway(QPainter& painter);
    void drawInfoOverlay(QPainter& painter);
    QPointF worldToScreen(double x, double y, double z) const;
};

#endif
