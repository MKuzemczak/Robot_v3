#ifndef MAINTAB_H
#define MAINTAB_H

#include <QWidget>
#include <QScrollArea>
#include <QLabel>
#include <QHBoxLayout>

#include "pointlistwidget.h"

class MainTab : public QWidget
{
    Q_OBJECT

    QLabel * label0,
            * label1;

    PointListWidget * actionList;

public:
    explicit MainTab(QWidget *parent = nullptr);

signals:

public slots:
};

#endif // MAINTAB_H
