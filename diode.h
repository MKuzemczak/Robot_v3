#ifndef DIODE_H
#define DIODE_H

#include <QWidget>
#include <QPainter>
#include <QString>
#include <QPaintEvent>
#include <QDebug>

class Diode : public QWidget
{
    Q_OBJECT

    QString label;

    QBrush color;
    bool state;
    int width, height;

public:
    explicit Diode(QString l, QWidget *parent = nullptr);

    void paintEvent(QPaintEvent *);

    bool getState();
signals:

public slots:
    void setOn();
    void setOff();
};

#endif // DIODE_H
