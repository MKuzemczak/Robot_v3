#ifndef PROGRAM_H
#define PROGRAM_H

#include <QWidget>
#include <QThread>
#include <QKeyEvent>

#include <iostream>

#include "serialcommunicator.h"
#include "actionmanager.h"

#define PROGRAM_DEBUG



class Program : public QObject
{
    Q_OBJECT

    Flags * flags;

    SerialPort arduinoPort;

    SerialCommunicatorThread * com;

    ActionManager * manager;

    Robot robot;

public:
    Program();
    ~Program();




signals:

public slots:
    void print(std::string s);
    void keyPressed(int);
    void testRobotInit();
    void testRun();
};

#endif // PROGRAM_H
