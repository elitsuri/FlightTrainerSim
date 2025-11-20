// File: Cockpit3DView.cpp
#include "Cockpit3DView.h"
#include "GlobalConfig.h"
#include <QPainter>
#include <QFont>
#include <cmath>

Cockpit3DView::Cockpit3DView(QWidget* parent) : QWidget(parent), m_aircraft(nullptr) {
    setMinimumSize(800, 600);
    setStyleSheet("background-color: black;");
}

void Cockpit3DView::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);
    drawHUD(painter);
}

void Cockpit3DView::drawHUD(QPainter& painter) {
    if (!m_aircraft) {
        painter.setPen(hudSecondaryColor());
        painter.setFont(QFont("Courier", 16, QFont::Bold));
        painter.drawText(rect(), Qt::AlignCenter, "NO AIRCRAFT DATA");
        return;
    }
    int w = width(), h = height(), cx = w / 2, cy = h / 2;
    drawAttitudeIndicator(painter, cx, cy, 300);
    drawAirspeedIndicator(painter, 50, cy - 150, 80, 300);
    drawAltimeter(painter, w - 130, cy - 150, 80, 300);
    drawHeadingIndicator(painter, cx - 200, h - 80, 400);
    drawVerticalSpeed(painter, w - 200, cy - 100, 40, 200);
    drawThrottleGauge(painter, 50, h - 200, 60, 150);
    drawCrosshair(painter, cx, cy);
    drawWarnings(painter);
    painter.setPen(hudSecondaryColor());
    painter.setFont(QFont("Courier", 10, QFont::Bold));
    QString info = QString("%1\nFUEL: %2 kg")
        .arg(QString::fromStdString(m_aircraft->flightModel()->getModelName()))
        .arg(static_cast<int>(m_aircraft->fuel()));
    painter.drawText(QRect(w - 250, 20, 230, 60), Qt::AlignLeft, info);
}

void Cockpit3DView::drawAttitudeIndicator(QPainter& painter, int cx, int cy, int size) {
    painter.save();
    int radius = size / 2;
    painter.setPen(QPen(hudPrimaryColor(), 3));
    painter.setBrush(QBrush(QColor(0, 20, 0, 200)));
    painter.drawEllipse(cx - radius, cy - radius, size, size);
    if (m_aircraft) {
        painter.translate(cx, cy);
        painter.rotate(-m_aircraft->bank());
        double pitch = m_aircraft->pitch();
        int pitchPixels = static_cast<int>(pitch * 3.0);
        painter.setPen(Qt::NoPen);
        painter.setBrush(QBrush(QColor(50, 100, 200, 150)));
        painter.drawRect(-radius, -radius, size, radius + pitchPixels);
        painter.setBrush(QBrush(QColor(139, 115, 85, 150)));
        painter.drawRect(-radius, pitchPixels, size, radius - pitchPixels);
        painter.setPen(QPen(hudPrimaryColor(), 3));
        painter.drawLine(-radius, pitchPixels, radius, pitchPixels);
        painter.resetTransform();
        painter.translate(cx, cy);
        painter.setPen(QPen(hudWarningColor(), 4));
        painter.drawLine(-50, 0, -15, 0);
        painter.drawLine(15, 0, 50, 0);
        painter.drawLine(0, -5, 0, 5);
    }
    painter.restore();
}

void Cockpit3DView::drawAirspeedIndicator(QPainter& painter, int x, int y, int width, int height) {
    painter.setPen(QPen(hudPrimaryColor(), 2));
    painter.setBrush(QBrush(QColor(0, 20, 0, 150)));
    painter.drawRect(x, y, width, height);
    if (m_aircraft) {
        double speed = m_aircraft->speed();
        painter.setFont(QFont("Courier", 12, QFont::Bold));
        int centerY = y + height / 2;
        for (int s = 0; s <= 500; s += 20) {
            int offset = static_cast<int>((speed - s) * 2.0);
            int tickY = centerY + offset;
            if (tickY >= y && tickY <= y + height) {
                painter.setPen(hudPrimaryColor());
                painter.drawLine(x + width - 15, tickY, x + width, tickY);
                if (s % 40 == 0) painter.drawText(x + 5, tickY + 5, QString::number(s));
            }
        }
        painter.setPen(QPen(hudWarningColor(), 2));
        painter.setBrush(QBrush(QColor(0, 0, 0, 200)));
        QRect speedBox(x + 10, centerY - 15, width - 20, 30);
        painter.drawRect(speedBox);
        painter.setPen(hudWarningColor());
        painter.drawText(speedBox, Qt::AlignCenter, QString::number(static_cast<int>(speed)));
    }
}

void Cockpit3DView::drawAltimeter(QPainter& painter, int x, int y, int width, int height) {
    painter.setPen(QPen(hudPrimaryColor(), 2));
    painter.setBrush(QBrush(QColor(0, 20, 0, 150)));
    painter.drawRect(x, y, width, height);
    if (m_aircraft) {
        double alt = m_aircraft->altitude();
        painter.setFont(QFont("Courier", 12, QFont::Bold));
        int centerY = y + height / 2;
        for (int a = 0; a <= 10000; a += 200) {
            int offset = static_cast<int>((alt - a) * 0.1);
            int tickY = centerY + offset;
            if (tickY >= y && tickY <= y + height) {
                painter.setPen(hudPrimaryColor());
                painter.drawLine(x, tickY, x + 15, tickY);
                if (a % 500 == 0) painter.drawText(x + 20, tickY + 5, QString::number(a));
            }
        }
        painter.setPen(QPen(hudWarningColor(), 2));
        painter.setBrush(QBrush(QColor(0, 0, 0, 200)));
        QRect altBox(x + 10, centerY - 15, width - 20, 30);
        painter.drawRect(altBox);
        painter.setPen(hudWarningColor());
        painter.drawText(altBox, Qt::AlignCenter, QString::number(static_cast<int>(alt)));
    }
}

void Cockpit3DView::drawHeadingIndicator(QPainter& painter, int x, int y, int width) {
    painter.setPen(QPen(hudPrimaryColor(), 2));
    painter.setBrush(QBrush(QColor(0, 20, 0, 150)));
    painter.drawRect(x, y, width, 50);
    if (m_aircraft) {
        double heading = m_aircraft->heading();
        painter.setFont(QFont("Courier", 10, QFont::Bold));
        int centerX = x + width / 2;
        for (int h = 0; h < 360; h += 10) {
            int offset = static_cast<int>((heading - h) * 3.0);
            if (offset > 180 * 3) offset -= 360 * 3;
            if (offset < -180 * 3) offset += 360 * 3;
            int tickX = centerX - offset;
            if (tickX >= x && tickX <= x + width) {
                painter.setPen(hudPrimaryColor());
                painter.drawLine(tickX, y + 40, tickX, y + 50);
                if (h % 30 == 0) {
                    QString label = QString::number(h / 10);
                    painter.drawText(tickX - 10, y + 25, label);
                }
            }
        }
        painter.setPen(QPen(hudWarningColor(), 3));
        painter.drawLine(centerX, y, centerX, y + 50);
        painter.drawText(centerX - 15, y + 15, QString("%1°").arg(static_cast<int>(heading)));
    }
}

void Cockpit3DView::drawVerticalSpeed(QPainter& painter, int x, int y, int width, int height) {
    painter.setPen(QPen(hudPrimaryColor(), 2));
    painter.setBrush(QBrush(QColor(0, 20, 0, 150)));
    painter.drawRect(x, y, width, height);
    if (m_aircraft) {
        double vs = m_aircraft->verticalSpeed();
        int centerY = y + height / 2;
        int pointerY = centerY - static_cast<int>(vs * 4.0);
        pointerY = std::clamp(pointerY, y, y + height);
        painter.setPen(QPen(hudWarningColor(), 3));
        painter.drawLine(x, pointerY, x + width, pointerY);
    }
}

void Cockpit3DView::drawThrottleGauge(QPainter& painter, int x, int y, int width, int height) {
    painter.setPen(QPen(hudPrimaryColor(), 2));
    painter.setBrush(QBrush(QColor(0, 20, 0, 150)));
    painter.drawRect(x, y, width, height);
    if (m_aircraft) {
        double throttle = m_aircraft->controls().throttle;
        int fillHeight = static_cast<int>(throttle * height);
        painter.setBrush(QBrush(hudPrimaryColor()));
        painter.drawRect(x + 5, y + height - fillHeight, width - 10, fillHeight);
        painter.setFont(QFont("Courier", 10, QFont::Bold));
        painter.setPen(hudSecondaryColor());
        painter.drawText(x, y + height + 15, QString("THR %1%").arg(static_cast<int>(throttle * 100)));
    }
}

void Cockpit3DView::drawWarnings(QPainter& painter) {
    if (!m_aircraft) return;
    QStringList warnings;
    if (m_aircraft->isStalled()) warnings << "STALL";
    if (m_aircraft->fuel() < 100.0) warnings << "LOW FUEL";
    if (m_aircraft->altitude() < 50 && !m_aircraft->isOnGround()) warnings << "ALTITUDE";
    if (!warnings.isEmpty()) {
        painter.setFont(QFont("Courier", 16, QFont::Bold));
        painter.setPen(hudCriticalColor());
        int y = 60;
        for (const auto& warning : warnings) {
            painter.drawText(20, y, warning);
            y += 30;
        }
    }
}

void Cockpit3DView::drawCrosshair(QPainter& painter, int cx, int cy) {
    painter.setPen(QPen(hudWarningColor(), 2));
    int size = 20;
    painter.drawLine(cx - size, cy, cx + size, cy);
    painter.drawLine(cx, cy - size, cx, cy + size);
    painter.drawEllipse(cx - 3, cy - 3, 6, 6);
}
