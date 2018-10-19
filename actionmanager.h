#pragma once

#include <QThread>
#include <QApplication>
#include <QTimer>


#include "baseaction.h"
#include "lista.h"

//#define DEBUG_ACTION_MANAGER

class ActionManager : public QObject
{
    Q_OBJECT

    Lista<BaseAction*, Eigen::aligned_allocator<BaseAction*>> actions;
    bool stepInProgress,
        checkCalculations,
        started,
        calculationsRunning;
    int actionCntr;

    Robot* robotPtr;

    Flags * flags;
    SerialPort * arduinoPort;

    BaseAction * currentlyCalculated;

    QThread calculationThread;

public:
    ActionManager();
    ActionManager(Flags * f, SerialPort * arduino, QObject * parent);
    ~ActionManager();

    void calculations();



    int size();

    void addStraightLineMovAction(Eigen::Vector3d & start,
                                  Eigen::Vector3d & dest);

    void addFreeMovAction(Eigen::Vector3d & dest);

    void addArchMovAction(Eigen::Vector3d start,
                          Eigen::Vector3d inter,
                          Eigen::Vector3d dest);

    void addConstTCPOrientAction(Eigen::Vector3d & start,
                                 Eigen::Vector3d & dest);

    void addSetSingleJointAction(int joint, int thetaDeg);

    void addGripperAction(int set);

    bool isCheckCalculations();

    void setRobotPtr(Robot * ptr);

    bool isStarted();

    void setFlagsPtr(Flags * ptr);
    void setArduinoPortPtr(SerialPort * ptr);

    void clear()
    {
        actions.clear();

        stepInProgress = false;
        checkCalculations = false;
        started = false;

        actionCntr = 0;

        currentlyCalculated = nullptr;
    }

public slots:
    void stopCalculationThreadFinished();
    void stopCalculationThreadFailed();
    void nextStep();
    void stop();
    bool start();
    void deleteAction(int);

signals:
    void startActionCalculations(Robot *);
    void writeToTerminal(QString);
    void writeToTerminal(int);
    void writeToTerminal(double);
    void writeToTerminal(char const *);
    void writeToTerminal(char);
    void writeToTerminal(std::string);

};

