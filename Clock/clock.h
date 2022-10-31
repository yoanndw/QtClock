#ifndef CLOCK_H
#define CLOCK_H

#include <QColor>
#include <QPaintEvent>
#include <QTime>
#include <QTimer>
#include <QWidget>

static const double HOUR_HAND_WIDTH_RATIO = 1.0 / 30.0;
static const double HOUR_HAND_LENGTH_RATIO = 0.5 * 0.5; // par rapport à la moitié de la fenêtre

static const double MINUTE_HAND_WIDTH_RATIO = 1.0 / 35.0;
static const double MINUTE_HAND_LENGTH_RATIO = 0.5 * 0.8; // par rapport à la moitié de la fenêtre

static const double SECOND_HAND_LENGTH_RATIO = 0.5 * 0.85; // par rapport à la moitié de la fenêtre

// Contours
static const double EDGE_DIST_RATIO = 0.5 * 0.9;
static const double SHORT_EDGE_RADIUS_RATIO = 0.5 * 0.007;
static const double LONG_EDGE_RADIUS_RATIO = 0.5 * 0.02;

// Nombres
static const double NB_DIST_RATIO = EDGE_DIST_RATIO - 0.02;

// Centre
static const double CENTER_RADIUS_RATIO = HOUR_HAND_WIDTH_RATIO + 0.01;

static const double FONT_SZ_RATIO = CENTER_RADIUS_RATIO - 0.01;

//static const double LONG_EDGE_LENGTH_RATIO = 0.5 * 0.1;
//static const double SHORT_EDGE_LENGTH_RATIO = 0.5 * 0.07;

class Clock : public QWidget
{
    Q_OBJECT

    double m_winDimension;

    QColor m_bgColor;

    QColor m_grandeAiguilleColor;
    QColor m_petiteAiguilleColor;
    QColor m_trotteuseColor;

    QColor m_centerColor;
    QColor m_textColor;

    QColor m_longEdgesColor;
    QColor m_shortEdgesColor;

    QPoint m_minutePoints[3];
    QPoint m_hourPoints[3];
    double m_secondLineLength;

    QTime m_time;
    double m_hour;

    double m_grandeAiguilleAngle;
    double m_petiteAiguilleAngle;
    double m_trotteuseAngle;

    QTimer* m_timer;

public:
    Clock(QWidget *parent = 0);

    void updateHour();
    void updatePoints();

    void paintEvent(QPaintEvent*) override;

    void drawGrandeAiguille(QPainter& painter);
    void drawPetiteAiguille(QPainter& painter);
    void drawTrotteuse(QPainter& painter);

    void drawCenter(QPainter& painter);
    void drawCenterText(QPainter& painter);

    void drawEdges(QPainter& painter);

    void drawNumbers(QPainter& painter);

signals:

public slots:
};

#endif // CLOCK_H
