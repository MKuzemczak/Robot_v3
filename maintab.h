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
#include "maincontrolwidget.h"
#include "program.h"

class MainTab : public QWidget
{
    Q_OBJECT

    Program * program;

    QLabel * label0,
            * label1;

    PointListWidget * pointList;
    ActionListWidget * actionList;
    MainControlWidget * mainControl;


public:
    explicit MainTab(Program * ptr, QWidget *parent = nullptr);

    void setProgramPtr(Program * ptr)
    {
        program = ptr;
    }

    PointListWidget * getPointList();

signals:

public slots:
};

#endif // MAINTAB_H
