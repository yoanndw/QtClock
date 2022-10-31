#include "clock.h"

#include <iostream>

#include <QtGlobal>
#include <QtMath>

#include <QDebug>
#include <QPainter>

Clock::Clock(QWidget *parent) : QWidget(parent)
{
    m_bgColor = QColor(3, 169, 244);

    m_grandeAiguilleColor = QColor(245, 0, 87);
    m_petiteAiguilleColor = QColor(255, 64, 129);
    m_trotteuseColor = QColor(255, 128, 171);

    //m_centerColor = QColor(255, 235, 59);
    m_centerColor = QColor(255, 128, 171);
    m_textColor = QColor(74, 20, 140);

    m_longEdgesColor = QColor(255, 255, 255);
    m_shortEdgesColor = QColor(255, 255, 255);

    updateHour();
    updatePoints();

    // Timer
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, QOverload<>::of(&Clock::update));

    // ATTENTION: ne pas oublier l'intervalle et de start
    m_timer->setInterval(1000);
    m_timer->start();
}

void Clock::updateHour()
{
    m_time = QTime::currentTime();

    m_hour = m_time.hour();
    double minute = m_time.minute();
    double second = m_time.second();

    // L'aiguille de l'heure prend en compte aussi les minutes (12h00 != 12h50)
    m_hour += minute / 60.0;

    m_petiteAiguilleAngle = (m_hour - 12) / 12 * 360;
    m_grandeAiguilleAngle = minute / 60 * 360;
    m_trotteuseAngle = second / 60 * 360;
}

void Clock::updatePoints()
{
    m_winDimension = qMin(width(), height());

    m_hourPoints[0] = QPoint(-m_winDimension * HOUR_HAND_WIDTH_RATIO, 0);
    m_hourPoints[1] = QPoint(0, -m_winDimension * HOUR_HAND_LENGTH_RATIO);
    m_hourPoints[2] = QPoint(m_winDimension * HOUR_HAND_WIDTH_RATIO, 0);

    m_minutePoints[0] = QPoint(-m_winDimension * MINUTE_HAND_WIDTH_RATIO, 0);
    m_minutePoints[1] = QPoint(0, -m_winDimension * MINUTE_HAND_LENGTH_RATIO);
    m_minutePoints[2] = QPoint(m_winDimension * MINUTE_HAND_WIDTH_RATIO, 0);

    m_secondLineLength = m_winDimension * SECOND_HAND_LENGTH_RATIO;
}

void Clock::paintEvent(QPaintEvent*)
{
    updateHour();
    updatePoints();

    QPainter painter(this);
    painter.fillRect(0, 0, width(), height(), m_bgColor);
    painter.translate(width() / 2, height() / 2);

    painter.rotate(m_trotteuseAngle);
    drawTrotteuse(painter);
    painter.rotate(-m_trotteuseAngle);

    painter.rotate(m_grandeAiguilleAngle);
    drawGrandeAiguille(painter);
    painter.rotate(-m_grandeAiguilleAngle);

    painter.rotate(m_petiteAiguilleAngle);
    drawPetiteAiguille(painter);
    painter.rotate(-m_petiteAiguilleAngle);

    drawCenter(painter);
    drawCenterText(painter);

    painter.save();
    drawEdges(painter);
    painter.restore();

    // TODO: les nombres ne sont pas exactement bien positionnés
    //drawNumbers(painter);
}

void Clock::drawGrandeAiguille(QPainter& painter)
{
    painter.setPen(m_grandeAiguilleColor);
    painter.setBrush(m_grandeAiguilleColor);
    painter.drawPolygon(m_minutePoints, 3);
}

void Clock::drawPetiteAiguille(QPainter& painter)
{
    painter.setPen(m_petiteAiguilleColor);
    painter.setBrush(m_petiteAiguilleColor);
    painter.drawPolygon(m_hourPoints, 3);
}

void Clock::drawTrotteuse(QPainter& painter)
{
    painter.setPen(m_trotteuseColor);
    painter.setBrush(m_trotteuseColor);
    painter.drawLine(0, 0, 0, -m_secondLineLength);
}

void Clock::drawCenter(QPainter& painter)
{
    painter.setPen(m_centerColor);
    painter.setBrush(m_centerColor);

    double radius = m_winDimension * CENTER_RADIUS_RATIO;
    painter.drawEllipse(QPointF(0, 0), radius, radius);
}

void Clock::drawCenterText(QPainter& painter)
{
    QString text = (m_hour < 12)
            ? QString("AM")
            : QString("PM");

    QFont font = painter.font();
    font.setPixelSize(m_winDimension * FONT_SZ_RATIO);
    painter.setFont(font);

    double centerCircleW = m_winDimension * CENTER_RADIUS_RATIO * 2;
    QRect textRect = QRect(-centerCircleW / 2, -centerCircleW / 2, centerCircleW, centerCircleW);

    painter.setPen(m_textColor);
    painter.drawText(textRect, Qt::AlignCenter, text);
}

void Clock::drawEdges(QPainter& painter)
{
    for (int i = 0; i < 60; i++)
    {
        bool longEdge = i % 5 == 0;

        painter.setPen(m_longEdgesColor);
        if (longEdge)
            painter.setBrush(m_bgColor);
        else
            painter.setBrush(m_longEdgesColor);

        double startY = -m_winDimension * EDGE_DIST_RATIO;

        double radiusRatio = longEdge
                ? LONG_EDGE_RADIUS_RATIO
                : SHORT_EDGE_RADIUS_RATIO;

        double radius = m_winDimension * radiusRatio;
        painter.drawEllipse(QPointF(0, startY), radius, radius);

        painter.rotate(6);
    }
}

void Clock::drawNumbers(QPainter& painter)
{
    QFont font = painter.font();
    font.setPixelSize(m_winDimension * FONT_SZ_RATIO);
    painter.setFont(font);

    painter.setPen(m_textColor);

    double dist = m_winDimension * NB_DIST_RATIO;

    double angle = -M_PI_2 + M_PI / 6.0;
    for (int i = 1; i <= 12; i++)
    {
        double startX = dist * qCos(angle);
        double startY = dist * qSin(angle);

        QString text = QString::number(i);
        QRect bounds = painter.boundingRect(QRect(startX, startY, 100, 100), 0, text);
        //double x = startX - bounds.width() / 2.0;
        //double y = startY - bounds.height() / 2.0;

        painter.drawText(bounds.center(), text);

        angle += M_PI / 6.0;
    }
}
