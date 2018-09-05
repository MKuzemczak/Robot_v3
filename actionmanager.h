#pragma once


#include "baseaction.h"
#include "lista.h"

//#define DEBUG_ACTION_MANAGER

class ActionManager
{
    Lista<BaseAction*> actions;
    bool stepInProgress,
        checkCalculations,
        started;
    int actionCntr;

    Robot* robotPtr;

    Flags * flags;
    SerialPort * arduinoPort;
public:
    ActionManager();
    ActionManager(Flags * f, SerialPort * arduino);
    ~ActionManager();

    bool start();
    void nextStep();
    void calculations();


    int size()
    {
        return actions.size();
    }

    void addStraightLineMovAction(Eigen::Vector3d & start,
                                  Eigen::Vector3d & dest)
    {
        actions.push_back(new StraightLineMovAction(start, dest, arduinoPort, flags));
    }

    void addFreeMovAction(Eigen::Vector3d & dest)
    {
        actions.push_back(new FreeMovAction(dest, arduinoPort, flags));
    }

    void addArchMovAction(Eigen::Vector3d start,
                          Eigen::Vector3d inter,
                          Eigen::Vector3d dest)
    {
        actions.push_back(new ArchMovAction(start, inter, dest, arduinoPort, flags));
    }

    void addConstTCPOrientAction(Eigen::Vector3d & start,
                                 Eigen::Vector3d & dest)
    {
        actions.push_back(new ConstTCPOrientAction(start, dest, arduinoPort, flags));
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

    void stop()
    {
        started = false;
    }

    void setFlagsPtr(Flags * ptr);
    void setArduinoPortPtr(SerialPort * ptr);
};

