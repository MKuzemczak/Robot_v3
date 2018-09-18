#pragma once

#include <QThread>
#include <QApplication>


#include "baseaction.h"
#include "lista.h"

#define DEBUG_ACTION_MANAGER

class ActionManager : public QObject
{
    Q_OBJECT

    Lista<BaseAction*> actions;
    bool stepInProgress,
        checkCalculations,
        started;
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



    int size()
    {
        return static_cast<int>(actions.size());
    }

    void addStraightLineMovAction(Eigen::Vector3d & start,
                                  Eigen::Vector3d & dest)
    {
        actions.push_back(new StraightLineMovAction(start, dest, arduinoPort, flags));
        actions[static_cast<int>(actions.size()) - 1]->setParentThreadPtr(this->thread());
    }

    void addFreeMovAction(Eigen::Vector3d & dest)
    {
        actions.push_back(new FreeMovAction(dest, arduinoPort, flags));
        actions[static_cast<int>(actions.size()) - 1]->setParentThreadPtr(this->thread());
    }

    void addArchMovAction(Eigen::Vector3d start,
                          Eigen::Vector3d inter,
                          Eigen::Vector3d dest)
    {
        actions.push_back(new ArchMovAction(start, inter, dest, arduinoPort, flags));
        actions[static_cast<int>(actions.size()) - 1]->setParentThreadPtr(this->thread());
    }

    void addConstTCPOrientAction(Eigen::Vector3d & start,
                                 Eigen::Vector3d & dest)
    {
        actions.push_back(new ConstTCPOrientAction(start, dest, arduinoPort, flags));
        actions[static_cast<int>(actions.size()) - 1]->setParentThreadPtr(this->thread());

#ifdef DEBUG_ACTION_MANAGER
        qDebug() << "ActionManager::addConstTCPOrientAction() : added thread: "
                 << actions[static_cast<int>(actions.size()) - 1]->thread();
#endif
    }

    void addSetSingleJointAction(int joint, int thetaDeg)
    {
        actions.push_back(new SetSingleJointAction(joint, thetaDeg, arduinoPort, flags));
        actions[static_cast<int>(actions.size()) - 1]->setParentThreadPtr(this->thread());
    }

    bool isCheckCalculations()
    {
        return checkCalculations;
    }

    void setRobotPtr(Robot * ptr)
    {
        robotPtr = ptr;
    }

    bool isStarted()
    {
        return started;
    }

    void setFlagsPtr(Flags * ptr);
    void setArduinoPortPtr(SerialPort * ptr);

public slots:
    void stopCalculationThread();
    void nextStep();
    void stop();
    bool start();

signals:
    void startActionCalculations(Robot *);
};

