#ifndef MAINTAB_H
#define MAINTAB_H

#include <QWidget>
#include <QScrollArea>
#include <QLabel>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QSplitter>

#include "pointlistwidget.h"
#include "actionlistwidget.h"

class MainTab : public QWidget
{
    Q_OBJECT

    QLabel * label0,
            * label1;

    PointListWidget * pointList;
    ActionListWidget * actionList;

public:
    explicit MainTab(QWidget *parent = nullptr);

signals:

public slots:
};

#endif // MAINTAB_H
