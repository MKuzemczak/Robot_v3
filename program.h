#ifndef PROGRAM_H
#define PROGRAM_H

#include <QWidget>
#include <QThread>
#include <QKeyEvent>
#include <QTextStream>

#include <iostream>

#include "serialcommunicator.h"
#include "actionmanager.h"
#include "pointlistwidget.h"
#include "terminal.h"
#include "joystick.h"

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

    Eigen::Vector3d robotBase;

public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

    Program();
    ~Program();

    Joystick * joystick;

    void setPointListPtr(PointListWidget * ptr)
    {
        pointList = ptr;
    }


    bool isSerialConnected();
    void scanConfig();

signals:
    void robotSet(int, int, int);
    void portConnected();
    void portDisconnected();
    void writeToTerminal(QString);
    void writeToTerminal(int);
    void writeToTerminal(double);
    void writeToTerminal(char);
    void writeToTerminal(char const *);
    void writeToTerminal(std::string);
    void started();
    void stopped();

public slots:
    void print(std::string s);
    void keyPressed(int);
    void setRobot(int, int, int);
    void testRobotInit();
    void testRun();
    void addAction(ActionType type, QString info);
    void deleteAction(int);
    void startSequence();
    void stop();
};

#endif // PROGRAM_H
