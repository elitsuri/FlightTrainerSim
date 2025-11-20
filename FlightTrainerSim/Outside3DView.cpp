// File: Outside3DView.cpp
#include "Outside3DView.h"
#include <QPainter>
#include <QFont>
#include <cmath>

Outside3DView::Outside3DView(QWidget* parent)
    : QWidget(parent), m_aircraft(nullptr), m_environment(nullptr)
    , m_cameraDistance(500.0), m_cameraAngle(30.0) {
    setMinimumSize(800, 600);
}

void Outside3DView::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    drawScene(painter);
}

void Outside3DView::drawScene(QPainter& painter) {
    drawSkyAndGround(painter);
    if (m_environment) { drawRunway(painter); drawWaypoints(painter); }
    if (m_aircraft) { drawFlightPath(painter); drawAircraft(painter, width() / 2, height() / 2); }
    drawInfoOverlay(painter);
}

void Outside3DView::drawSkyAndGround(QPainter& painter) {
    int h = height(), horizonY = h / 2;
    if (m_aircraft) horizonY -= static_cast<int>(m_aircraft->pitch() * 2.0);
    QLinearGradient skyGradient(0, 0, 0, horizonY);
    skyGradient.setColorAt(0.0, QColor(20, 50, 150));
    skyGradient.setColorAt(1.0, QColor(135, 206, 235));
    painter.fillRect(0, 0, width(), horizonY, skyGradient);
    QLinearGradient groundGradient(0, horizonY, 0, h);
    groundGradient.setColorAt(0.0, QColor(139, 115, 85));
    groundGradient.setColorAt(1.0, QColor(101, 67, 33));
    painter.fillRect(0, horizonY, width(), h - horizonY, groundGradient);
    painter.setPen(QPen(QColor(255, 255, 255, 150), 2));
    painter.drawLine(0, horizonY, width(), horizonY);
}

void Outside3DView::drawAircraft(QPainter& painter, int cx, int cy) {
    if (!m_aircraft) return;
    painter.save();
    painter.translate(cx, cy);
    painter.rotate(m_aircraft->bank());
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(200, 200, 200));
    QPolygon fuselage;
    fuselage << QPoint(-5, -40) << QPoint(5, -40) << QPoint(8, 30) << QPoint(-8, 30);
    painter.drawPolygon(fuselage);
    painter.setBrush(QColor(180, 180, 200));
    QPolygon wings;
    wings << QPoint(-80, 0) << QPoint(-10, -5) << QPoint(10, -5) << QPoint(80, 0)
          << QPoint(80, 10) << QPoint(10, 5) << QPoint(-10, 5) << QPoint(-80, 10);
    painter.drawPolygon(wings);
    painter.restore();
}

void Outside3DView::drawFlightPath(QPainter& painter) {
    if (!m_aircraft) return;
    const auto& path = m_aircraft->flightPath();
    if (path.size() < 2) return;
    painter.setPen(QPen(QColor(0, 255, 0, 180), 2, Qt::DashLine));
    QPoint prevScreen;
    bool firstPoint = true;
    for (const auto& pos : path) {
        QPointF screen = worldToScreen(pos.x, pos.y, pos.z);
        QPoint screenInt(static_cast<int>(screen.x()), static_cast<int>(screen.y()));
        if (!firstPoint && screenInt.x() >= 0 && screenInt.x() < width() &&
            screenInt.y() >= 0 && screenInt.y() < height()) {
            painter.drawLine(prevScreen, screenInt);
        }
        prevScreen = screenInt;
        firstPoint = false;
    }
}

void Outside3DView::drawWaypoints(QPainter& painter) {
    if (!m_environment) return;
    const auto& waypoints = m_environment->waypoints();
    painter.setFont(QFont("Arial", 9, QFont::Bold));
    for (const auto& wp : waypoints) {
        QPointF screen = worldToScreen(wp.x, wp.y, wp.altitude);
        if (screen.x() >= 0 && screen.x() < width() && screen.y() >= 0 && screen.y() < height()) {
            painter.setPen(QPen(QColor(255, 255, 0), 2));
            painter.setBrush(QColor(255, 255, 0, 150));
            painter.drawEllipse(QPointF(screen), 12, 12);
            painter.setPen(QColor(255, 255, 255));
            painter.drawText(static_cast<int>(screen.x()) + 15, static_cast<int>(screen.y()) + 5,
                           QString::fromStdString(wp.name));
        }
    }
}

void Outside3DView::drawRunway(QPainter& painter) {
    if (!m_environment) return;
    const auto& airfields = m_environment->airfields();
    if (airfields.empty()) return;
    const auto& runway = airfields[0];
    QPointF rwyStart = worldToScreen(runway.x, runway.y, 0);
    QPointF rwyEnd = worldToScreen(runway.x + runway.runwayLength, runway.y, 0);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(60, 60, 70));
    QPolygonF runwayShape;
    runwayShape << QPointF(rwyStart.x() - 30, rwyStart.y() - 50)
                << QPointF(rwyEnd.x() - 30, rwyEnd.y() - 50)
                << QPointF(rwyEnd.x() + 30, rwyEnd.y() + 50)
                << QPointF(rwyStart.x() + 30, rwyStart.y() + 50);
    painter.drawPolygon(runwayShape);
}

void Outside3DView::drawInfoOverlay(QPainter& painter) {
    if (!m_aircraft) return;
    painter.setFont(QFont("Courier", 10, QFont::Bold));
    painter.setPen(QColor(255, 255, 255, 200));
    QString info = QString("Position: (%1, %2)\nAltitude: %3 ft\nSpeed: %4 kts\nHeading: %5°")
        .arg(static_cast<int>(m_aircraft->position().x))
        .arg(static_cast<int>(m_aircraft->position().y))
        .arg(static_cast<int>(m_aircraft->altitude()))
        .arg(static_cast<int>(m_aircraft->speed()))
        .arg(static_cast<int>(m_aircraft->heading()));
    painter.drawText(QRect(10, 10, 250, 100), Qt::AlignLeft | Qt::AlignTop, info);
}

QPointF Outside3DView::worldToScreen(double x, double y, double z) const {
    if (!m_aircraft) return QPointF(width() / 2, height() / 2);
    double camX = m_aircraft->position().x - m_cameraDistance * std::cos(m_cameraAngle * M_PI / 180.0);
    double camY = m_aircraft->position().y - m_cameraDistance * std::sin(m_cameraAngle * M_PI / 180.0);
    double camZ = m_aircraft->altitude() + 200;
    double dx = x - camX, dy = y - camY, dz = z - camZ;
    double scale = 0.5;
    double screenX = width() / 2 + dx * scale;
    double screenY = height() / 2 + dy * scale - dz * scale;
    return QPointF(screenX, screenY);
}
