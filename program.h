#ifndef PROGRAM_H
#define PROGRAM_H

#include <QWidget>
#include <QThread>
#include <QKeyEvent>

#include <iostream>

#include "serialcommunicator.h"
#include "actionmanager.h"
#include "pointlistwidget.h"

//#define PROGRAM_DEBUG



class Program : public QObject
{
    Q_OBJECT

    Flags * flags;

    SerialCommunicatorThread * com;

    ActionManager * manager;

    SerialPort * arduinoPort;

    Robot robot;

    PointListWidget * pointList;

public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

    Program();
    ~Program();

    void setPointListPtr(PointListWidget * ptr)
    {
        pointList = ptr;
    }


signals:

public slots:
    void print(std::string s);
    void keyPressed(int);
    void testRobotInit();
    void testRun();
    void addAction(ActionType type, QString info);
};

#endif // PROGRAM_H
