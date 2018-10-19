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
#include "terminal.h"

class MainTab : public QWidget
{
    Q_OBJECT

    Program * program;

    QLabel * label0,
            * label1;

    PointListWidget * pointList;
    ActionListWidget * actionList;
    MainControlWidget * mainControl;

    Terminal * terminal;

    QPushButton * showTerminalButton;

    QSplitter * splitter,
              * splitter0,
              * splitter1;


public:
    explicit MainTab(Program * ptr, QWidget *parent = nullptr);

    void setProgramPtr(Program * ptr)
    {
        program = ptr;
    }

    PointListWidget * getPointList();
    void scanConfig();

signals:

public slots:
    void terminalTest();
    void resizeTerminal();

};

#endif // MAINTAB_H
