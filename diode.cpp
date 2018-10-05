#include "diode.h"

Diode::Diode(QString l, QWidget *parent) :
    QWidget(parent),
    color(QBrush(QColor(255, 0, 0, 255)))
{
    color.setColor(QColor(255, 0, 0, 255));
    height = 10;

    label = l;

    QFontMetrics fm = fontMetrics();
    width = fm.width(label) + 13;

    setGeometry(this->x(), this->y(), width, height);

    setMaximumHeight(15);
    setMinimumHeight(15);
    setMinimumWidth(width+20);

}

void Diode::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QFontMetrics fm = painter.fontMetrics();
    int w = fm.width(label),
            h = fm.height();
    QRect r = rect();

    r.moveTo(r.x()+(w/2)+3 + (r.width()/2), r.y() + (r.height()/2) - h/2 + 2);
    r.setSize(QSize(height, height));

    painter.drawText(rect(), Qt::AlignCenter, label);
    painter.setBrush(color);
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(r);
}

void Diode::setOn()
{
    state = true;
    color.setColor(QColor(0, 255, 0, 255));
    update();
}

void Diode::setOff()
{
    state = false;
    color.setColor(QColor(255, 0, 0, 255));
    update();
}

bool Diode::getState()
{
    return state;
}
